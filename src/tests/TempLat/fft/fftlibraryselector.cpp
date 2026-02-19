
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/device_iteration.h"
#include "TempLat/util/almostequal.h"
#include "TempLat/util/ndloop.h"
#include "TempLat/fft/fftmpidomainsplit.h"
#include <iostream>

namespace TempLat
{
  template <size_t NDim, typename T>
  DEVICE_FUNCTION T coordinateToValue(device::IdxArray<NDim> complexMemCoordinate,
                                      const LayoutStruct<NDim> &currentLayout, bool fourierSpace)
  {
    const auto nGrid = currentLayout.getGlobalSizes();

    // now, in this routine we walk the memory lineary, not caring about complex stuff. Hence vPosctv[2] /= 2;

    bool isImaginaryPart = complexMemCoordinate[NDim - 1] % 2;
    if (fourierSpace) complexMemCoordinate[NDim - 1] /= 2;

    device::IdxArray<NDim> spaceCoordinate{};
    device::apply(
        [&](const auto &...idx) { currentLayout.putSpatialLocationFromMemoryIndexInto(spaceCoordinate, idx...); },
        complexMemCoordinate);

    device::IdxArray<NDim> hermitianPartnerCoordinate{};
    for (size_t i = 0; i < NDim; ++i)
      hermitianPartnerCoordinate[i] = spaceCoordinate[i];

    ptrdiff_t imaginaryPartSign = 1;

    auto hermQual = currentLayout.getHermitianPartners().qualify(spaceCoordinate);
    currentLayout.getHermitianPartners().putHermitianPartner(spaceCoordinate, hermitianPartnerCoordinate);

    if (fourierSpace)
      if (isImaginaryPart) {
        if (hermQual == HermitianRedundancy::realValued) {
          imaginaryPartSign = 0;
        } else if (hermQual == HermitianRedundancy::negativePartner) {
          imaginaryPartSign = -1;
        } else if (hermQual == HermitianRedundancy::positivePartner) {
          imaginaryPartSign = 1;
        }
      }

    T result = 0.;
    for (const auto &it : hermitianPartnerCoordinate) { // WARNING: works only for isotropic grids
      result = result * nGrid[0] + (it + nGrid[0] / 2);
    }

    return imaginaryPartSign * result;
  };

  template <size_t NDim, typename T>
  bool checkMem(const T norm, const T val, const device::IdxArray<NDim> vPos, const LayoutStruct<NDim> &currentLayout,
                bool fourierSpace)
  {
    const auto &memorySizes = currentLayout.getSizesInMemory();

    // sayMPI << "checkMem " << pos << "\n";
    T valueShouldBe = coordinateToValue<NDim, T>(vPos, currentLayout, fourierSpace);
    if (!fourierSpace)
      if (vPos[NDim - 1] >= memorySizes[NDim - 1] - 2) return true;

    bool thisCheck = AlmostEqual(valueShouldBe, norm * val, std::is_same<T, float>::value ? 0.1 : 1e-4);

    if (!thisCheck) {
      std::string vPosString = "{{ ";
      for (const auto &it : vPos)
        vPosString += std::to_string(it) + " ";
      vPosString += "}}";
      sayMPI << "Not equal: " << vPosString << " -> " << valueShouldBe << " != " << norm * val << " = " << norm << " * "
             << val;
    }
    return thisCheck;
  };

  template <size_t NDim, typename T>
  void test_r2c_c2r(TempLat::TDDAssertion &tdd, const TempLat::device::IdxArray<NDim> nGrid)
  {
    auto split = FFTMPIDomainSplit<NDim>::makeDomainDecomposition(MPICommReference().size(), NDim);
    auto mGroup_ = MPICartesianGroup(NDim, split);

    device::IdxArray<NDim> nGridPoints;
    for (size_t i = 0; i < NDim; ++i)
      nGridPoints[i] = nGrid[i];

    FFTLibrarySelector<NDim> ffter(mGroup_, nGridPoints);
    sayMPI << "Using backend: " << ffter.getBackend() << " NDim = " << NDim << ", grid size = " << nGrid[0] << "\n";

    auto fftLayout = ffter.getLayout();
    MemoryBlock<NDim, T> mem(fftLayout.getMinimalMemorySize());

    const auto configLayout = fftLayout.configurationSpace;

    auto localSizes = configLayout.getLocalSizes();
    localSizes[NDim - 1] += 2; // account for the padding in the r2c/cr2 case
    auto mem_view = mem.getNDView(localSizes);
    // Fill the memory with known values.
    device::iteration::foreach (
        "Set a point", configLayout, DEVICE_LAMBDA(const device::IdxArray<NDim> &idx) {
          device::apply(
              [&](const auto &...i) { mem_view(i...) = coordinateToValue<NDim, T>(idx, configLayout, false); }, idx);
        });

    sayMPI << "About to do FFT.\n";
    ffter.r2c(mem);

    {
      // Ensure that the data actually changed.
      int after_first = 0;
      const auto result_view = mem.getNDHostView(localSizes);
      NDLoop<NDim>(result_view, [&](const auto &...i) {
        device::IdxArray<NDim> vPos = {i...};
        auto old = coordinateToValue<NDim, T>(vPos, configLayout, false);
        if (abs(result_view(i...) - old) > 0.1) after_first += 1;
      });
      tdd.verify(after_first > 0);
      if (after_first == 0) {
        sayMPI << "Failed for NDim: " << NDim << ", nGrid[0]: " << nGrid[0] << "\n";
      }
      mem.flagHostMirrorOutdated();
    }

    ffter.c2r(mem);
    sayMPI << "Finished FFT.\n";

    const T norm =
        1. / std::pow(nGrid[0], NDim) * fftLayout.getIntrinsicScales().r2c * fftLayout.getIntrinsicScales().c2r;

    // Verify that we recover the original data
    {
      int r2c_then_c2r = 0;
      const auto result_view = mem.getNDHostView(localSizes);
      NDLoop<NDim>(result_view, [&](const auto &...i) {
        device::IdxArray<NDim> vPos = {i...};
        if (!checkMem(norm, result_view(i...), vPos, configLayout, false)) r2c_then_c2r += 1;
      });
      tdd.verify(r2c_then_c2r == 0);
      if (r2c_then_c2r > 0) {
        sayMPI << "Failed for NDim: " << NDim << ", nGrid[0]: " << nGrid[0] << "\n"
               << "Number of faulty points: " << r2c_then_c2r << " out of " << mem.size() << "\n"
               << "Percentage of faulty points: " << (double)r2c_then_c2r / (double)mem.size() * 100. << " %\n";
      }
    }
  };

  struct FFTLibrarySelectorTester {
    template <typename T> static void TestBody(TDDAssertion &tdd)
    {
      /* int main already calls the mpi guard, which calls the FFT session guards. So this should be the second time. */
      tdd.verify(Throws<FFTLibraryDoubleInitializationException>([]() { getFFTSessionGuards(); }));

      // We test in 2,3,4 dimensions, and for grids 2^2, ..., 2^5.
      {
        constexpr size_t NDim = 2;
        sayMPI << "Testing FFTLibrarySelector for NDim = " << NDim << "\n";
        for (ptrdiff_t inGrid : device::IdxArray<4>{3, 4, 7, 10}) {
          device::IdxArray<NDim> nGrid;
          for (auto &it : nGrid)
            it = std::pow(2, inGrid);
          test_r2c_c2r<NDim, T>(tdd, nGrid);
        }
      }
      {
        constexpr size_t NDim = 3;
        sayMPI << "Testing FFTLibrarySelector for NDim = " << NDim << "\n";
        for (ptrdiff_t inGrid : device::IdxArray<3>{2, 3, 6}) {
          device::IdxArray<NDim> nGrid;
          for (auto &it : nGrid)
            it = std::pow(2, inGrid);
          test_r2c_c2r<NDim, T>(tdd, nGrid);
        }
      }
      {
        constexpr size_t NDim = 4;
        sayMPI << "Testing FFTLibrarySelector for NDim = " << NDim << "\n";
        for (ptrdiff_t inGrid : device::IdxArray<2>{2, 3}) {
          device::IdxArray<NDim> nGrid;
          for (auto &it : nGrid)
            it = std::pow(2, inGrid);
          test_r2c_c2r<NDim, T>(tdd, nGrid);
        }
      }
    }

    static void Test(TDDAssertion &tdd)
    {
#ifdef HAVE_FFTFLOAT
      TestBody<float>(tdd);
#endif
      TestBody<double>(tdd);
    }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FFTLibrarySelectorTester> test;
}
