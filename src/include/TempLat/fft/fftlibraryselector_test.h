#ifndef TEMPLAT_FFT_FFTLIBRARYSELECTOR_TEST_H
#define TEMPLAT_FFT_FFTLIBRARYSELECTOR_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/device.h"
#include "TempLat/util/almostequal.h"
#include "TempLat/fft/fftmpidomainsplit.h"

#include <iostream>

namespace TempLat
{
  template <size_t NDim, typename T>
  DEVICE_FUNCTION T coordinateToValue(const device::array<ptrdiff_t, NDim> vPosctv,
                                      const LayoutStruct<NDim> &currentLayout, bool fourierSpace)
  {
    device::array<ptrdiff_t, NDim> complexMemCoordinate{};
    for (size_t i = 0; i < NDim; ++i)
      complexMemCoordinate[i] = vPosctv[i];
    device::array<ptrdiff_t, NDim> spaceCoordinate{};

    const auto nGrid = currentLayout.getGlobalSizes();

    // now, in this routine we walk the memory lineary, not caring about complex stuff. Hence vPosctv[2] /= 2;

    bool isImaginaryPart = complexMemCoordinate[NDim - 1] % 2;

    if (fourierSpace) complexMemCoordinate[NDim - 1] /= 2;

    device::apply(
        [&](const auto &...idx) { currentLayout.putSpatialLocationFromMemoryIndexInto(spaceCoordinate, idx...); },
        complexMemCoordinate);

    device::array<ptrdiff_t, NDim> hermitianPartnerCoordinate{};
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
  bool checkMem(const T norm, const T val, const device::array<ptrdiff_t, NDim> vPos,
                const LayoutStruct<NDim> &currentLayout, bool fourierSpace)
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
  void test_c2r_r2c(TempLat::TDDAssertion &tdd, const TempLat::device::array<ptrdiff_t, NDim> nGrid)
  {
    auto split = FFTMPIDomainSplit<NDim>::makeDomainDecomposition(MPICommReference().size(), NDim);
    auto mGroup_ = MPICartesianGroup(NDim, split);
    std::array<ptrdiff_t, NDim> nGridPoints;
    for (size_t i = 0; i < NDim; ++i)
      nGridPoints[i] = nGrid[i];
    FFTLibrarySelector<NDim> ffter(mGroup_, nGridPoints);
    // ffter.setVerbose();
    sayMPI << "Using backend: " << ffter.getBackend() << " NDim = " << NDim << ", grid size = " << nGrid[0] << "\n";

    device::array<ptrdiff_t, NDim> nGridFourier = nGrid;
    nGridFourier[NDim - 1] = (nGridFourier[NDim - 1] / 2 + 1) * 2;

    auto layout = ffter.getLayout();
    MemoryBlock<NDim, T> mem(layout.getMinimalMemorySize());

    const auto currentLayout = layout.fourierSpace;

    // Fill the memory with known values.
    Kokkos::parallel_for(
        "Set a point", Kokkos::RangePolicy(0, mem.size()), DEVICE_LAMBDA(const size_t i) {
          device::array<ptrdiff_t, NDim> pos{};
          size_t acc = 1;
          for (size_t j = 0; j < NDim; ++j) {
            pos[NDim - 1 - j] = (i / acc) % nGridFourier[NDim - 1 - j];
            acc *= nGridFourier[NDim - 1 - j];
          }
          mem[i] = coordinateToValue<NDim, T>(pos, currentLayout, true);
        });

    // Print the field content for debugging
    if constexpr (NDim == 2) {
      if (nGrid[0] <= 8) {
        const auto result_view = mem.getRawHostView();
        std::cout << "Memory contents BEFORE c2r and r2c:\n";
        for (size_t i = 0; i < mem.size(); ++i) {
          std::cout /*<< "mem[" << i << "] = "*/ << std::setw(12) << result_view(i);
          if (i % nGridFourier[0] == nGridFourier[0] - 1)
            std::cout << std::endl;
          else
            std::cout << ", ";
        }
        std::cout << std::endl;
      }
    }

    sayMPI << "About to do c2r FFT.\n";
    ffter.c2r(mem);
    sayMPI << "Finished c2r.\n";

    // Print the field content for debugging
    if constexpr (NDim == 2) {
      if (nGrid[0] <= 8) {
        const auto result_view = mem.getRawHostView();
        std::cout << "Memory contents INTERMEDIATE c2r:\n";
        for (size_t i = 0; i < mem.size(); ++i) {
          std::cout /*<< "mem[" << i << "] = "*/ << std::setw(12) << result_view(i);
          if (i % nGridFourier[0] == nGridFourier[0] - 1)
            std::cout << std::endl;
          else
            std::cout << ", ";
        }
        std::cout << std::endl;
      }
    }

    // Just check that it's different
    {
      bool after_first = false;
      const auto result_view = mem.getRawHostView();
      for (size_t i = 0; i < mem.size(); ++i) {
        device::array<ptrdiff_t, NDim> vPos{};
        size_t acc = 1;
        for (size_t j = 0; j < NDim; ++j) {
          vPos[NDim - 1 - j] = (i / acc) % nGridFourier[NDim - 1 - j];
          acc *= nGridFourier[NDim - 1 - j];
        }
        auto old = coordinateToValue<NDim, T>(vPos, currentLayout, true);
        after_first = (abs(result_view(i) - old) > 0.1) || after_first;
      }
      tdd.verify(after_first);
      if (!after_first) sayMPI << "Failed for NDim: " << NDim << ", nGrid[0]: " << nGrid[0] << "\n";
    }

    sayMPI << "About to do r2c FFT.\n";
    ffter.r2c(mem);
    sayMPI << "Finished r2c FFT.\n";

    const T norm = 1. / std::pow(nGrid[0], NDim) * ffter.getLayout().getIntrinsicScales().r2c *
                   ffter.getLayout().getIntrinsicScales().c2r;

    // Print the field content for debugging
    if constexpr (NDim == 2) {
      if (nGrid[0] <= 8) {
        const auto result_view = mem.getRawHostView();
        std::cout << "Memory contents AFTER c2r and r2c (normalized):\n";
        for (size_t i = 0; i < mem.size(); ++i) {
          std::cout /*<< "mem[" << i << "] = "*/ << std::setw(12) << result_view(i);
          if (i % nGridFourier[0] == nGridFourier[0] - 1)
            std::cout << std::endl;
          else
            std::cout << ", ";
        }
        std::cout << std::endl;
      }
    }

    // Check that we recover the original data
    {
      bool c2r_then_r2c = true;
      const auto result_view = mem.getRawHostView();
      for (size_t i = 0; i < mem.size(); ++i) {
        device::array<ptrdiff_t, NDim> vPos{};
        size_t acc = 1;
        for (size_t j = 0; j < NDim; ++j) {
          vPos[NDim - 1 - j] = (i / acc) % nGridFourier[NDim - 1 - j];
          acc *= nGridFourier[NDim - 1 - j];
        }
        c2r_then_r2c = checkMem(norm, result_view(i), vPos, currentLayout, true) && c2r_then_r2c;
      }
      tdd.verify(c2r_then_r2c);
      if (!c2r_then_r2c) sayMPI << "Failed for NDim: " << NDim << ", nGrid[0]: " << nGrid[0] << "\n";
    }
  };

  template <size_t NDim, typename T>
  void test_r2c_c2r(TempLat::TDDAssertion &tdd, const TempLat::device::array<ptrdiff_t, NDim> nGrid)
  {
    auto split = FFTMPIDomainSplit<NDim>::makeDomainDecomposition(MPICommReference().size(), NDim);
    auto mGroup_ = MPICartesianGroup(NDim, split);
    std::array<ptrdiff_t, NDim> nGridPoints;
    for (size_t i = 0; i < NDim; ++i)
      nGridPoints[i] = nGrid[i];
    FFTLibrarySelector<NDim> ffter(mGroup_, nGridPoints);
    // ffter.setVerbose();
    std::cout << "Using backend: " << ffter.getBackend() << " NDim = " << NDim << ", grid size = " << nGrid[0] << "\n";

    auto layout = ffter.getLayout();
    MemoryBlock<NDim, T> mem(layout.getMinimalMemorySize());

    const auto currentLayout = layout.configurationSpace;
    const auto memorySizes = currentLayout.getSizesInMemory();

    // Fill the memory with known values.
    Kokkos::parallel_for(
        "Set a point", Kokkos::RangePolicy(0, mem.size()), DEVICE_LAMBDA(const size_t i) {
          device::array<ptrdiff_t, NDim> pos{};
          size_t acc = 1;
          for (size_t j = 0; j < NDim; ++j) {
            pos[NDim - 1 - j] = (i / acc) % memorySizes[NDim - 1 - j];
            acc *= memorySizes[NDim - 1 - j];
          }
          device::apply([&](const auto... idx) { mem[i] = coordinateToValue<NDim, T>(pos, currentLayout, false); },
                        pos);
        });

    sayMPI << "About to do FFT.\n";
    ffter.r2c(mem);
    ffter.c2r(mem);
    sayMPI << "Finished FFT.\n";

    const T norm = 1. / std::pow(nGrid[0], NDim) * ffter.getLayout().getIntrinsicScales().r2c *
                   ffter.getLayout().getIntrinsicScales().c2r;

    // Verify that we recover the original data
    {
      bool r2c_then_c2r = true;
      const auto result_view = mem.getRawHostView();
      for (size_t i = 0; i < mem.size(); ++i) {
        device::array<ptrdiff_t, NDim> vPos{};
        size_t acc = 1;
        for (size_t j = 0; j < NDim; ++j) {
          vPos[NDim - 1 - j] = (i / acc) % memorySizes[NDim - 1 - j];
          acc *= memorySizes[NDim - 1 - j];
        }
        r2c_then_c2r = checkMem(norm, result_view(i), vPos, currentLayout, false) && r2c_then_c2r;
      }
      tdd.verify(r2c_then_c2r);
      if (!r2c_then_c2r) sayMPI << "Failed for NDim: " << NDim << ", nGrid[0]: " << nGrid[0] << "\n";
    }
  };
} // namespace TempLat

template <size_t _NDim>
template <typename T>
inline void TempLat::FFTLibrarySelector<_NDim>::TestBody(TempLat::TDDAssertion &tdd)
{
  /* int main already calls the mpi guard, which calls the FFT session guards. So this should be the second time. */
  tdd.verify(Throws<FFTLibraryDoubleInitializationException>([]() { getFFTSessionGuards(); }));

  // We test in 2,3,4 dimensions, and for grids 2^4, ..., 2^5.
  constexpr_for<2, 5, 1>([&](auto i) {
    sayMPI << "Testing FFTLibrarySelector for NDim = " << decltype(i)::value << "\n";
    constexpr size_t NDim = decltype(i)::value;
    for (ptrdiff_t inGrid = 2; inGrid < 5; ++inGrid) {
      device::array<ptrdiff_t, NDim> nGrid;
      for (auto &it : nGrid)
        it = std::pow(2, inGrid);
      test_r2c_c2r<NDim, T>(tdd, nGrid);
      test_c2r_r2c<NDim, T>(tdd, nGrid);
    }
  });
}

template <size_t NDim> inline void TempLat::FFTLibrarySelector<NDim>::Test(TempLat::TDDAssertion &tdd)
{
#ifndef NOFFTFLOAT
  TestBody<float>(tdd);
#endif
  TestBody<double>(tdd);
}

#endif
