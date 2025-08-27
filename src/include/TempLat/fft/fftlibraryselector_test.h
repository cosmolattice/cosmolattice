#ifndef TEMPLAT_FFT_FFTLIBRARYSELECTOR_TEST_H
#define TEMPLAT_FFT_FFTLIBRARYSELECTOR_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"
#include "TempLat/fft/fftmpidomainsplit.h"
#include <functional>

template <size_t _NDim>
template <typename T>
inline void TempLat::FFTLibrarySelector<_NDim>::TestBody(TempLat::TDDAssertion &tdd)
{
  /* int main already calls the mpi guard, which calls the FFT session guards. So this should be the second time. */
  tdd.verify(Throws<FFTLibraryDoubleInitializationException>([]() { getFFTSessionGuards(); }));

  /* let's create some memory, FFT forth and back, and check that the result is close enough to the input, module the
   * normalization.. */

  auto &&myLittleLambda = [&tdd](const auto nGrid) {
    constexpr size_t NDim = nGrid.size();
    auto split = FFTMPIDomainSplit<NDim>::makeDomainDecomposition(MPICommReference().size(), NDim);
    auto mGroup_ = MPICartesianGroup(NDim, split);
    std::array<ptrdiff_t, NDim> nGridPoints;
    for (size_t i = 0; i < NDim; ++i)
      nGridPoints[i] = nGrid[i];
    FFTLibrarySelector<NDim> ffter(mGroup_, nGridPoints);
    ffter.setVerbose();

    auto layout = ffter.getLayout();
    // say << layout << "\n";
    MemoryBlock<NDim, T> mem(layout.getMinimalMemorySize());

    // We check once r2c ( i.e. start with configuration space, order == 0 ) and once c2r ( i.e. start with fourier
    // space, order == 1 ).
    for (int order = 0; order < 2; ++order) {
      auto &currentLayout = order ? layout.fourierSpace : layout.configurationSpace;
      auto memorySizes = currentLayout.getSizesInMemory();

      // manually implement 3d:
      auto coordinateToValue = KOKKOS_LAMBDA(const device::array<ptrdiff_t, NDim> &vPosctv)
      {
        device::array<ptrdiff_t, NDim> complexMemCoordinate{};
        for (size_t i = 0; i < NDim; ++i)
          complexMemCoordinate[i] = vPosctv[i];
        device::array<ptrdiff_t, NDim> spaceCoordinate{};

        // now, in this routine we walk the memory lineary, not caring about complex stuff. Hence vPosctv[2] /= 2;

        bool isImaginaryPart = complexMemCoordinate[NDim - 1] % 2;

        complexMemCoordinate[NDim - 1] /= 2;
        device::apply(
            [&](const auto &...idx) { currentLayout.putSpatialLocationFromMemoryIndexInto(spaceCoordinate, idx...); },
            complexMemCoordinate);

        device::array<ptrdiff_t, NDim> hermitianPartnerCoordinate{};
        for (size_t i = 0; i < NDim; ++i)
          hermitianPartnerCoordinate[i] = spaceCoordinate[i];

        ptrdiff_t imaginaryPartSign = 1;
        if (order) {
          auto hermQual = currentLayout.getHermitianPartners().qualify(spaceCoordinate);
          currentLayout.getHermitianPartners().putHermitianPartner(spaceCoordinate, hermitianPartnerCoordinate);

          if (isImaginaryPart) {
            if (hermQual == HermitianRedundancy::realValued) {
              imaginaryPartSign = 0;
            } else if (hermQual == HermitianRedundancy::negativePartner) {
              imaginaryPartSign = -1;
            } else if (hermQual == HermitianRedundancy::positivePartner) {
              imaginaryPartSign = 1;
            }
          }
        }

        T result = 0.;
        for (const auto &it : hermitianPartnerCoordinate) { // WARNING: works only for isotropic grids
          result = result * nGrid[0] + (it + nGrid[0] / 2);
        }

        return imaginaryPartSign * result;
      };

      // Fill the memory with known values.
      Kokkos::parallel_for(
          "Set a point", Kokkos::RangePolicy(0, mem.size()), KOKKOS_LAMBDA(const size_t i) {
            device::array<ptrdiff_t, NDim> pos{};
            size_t acc = 1;
            for (size_t j = 0; j < NDim; ++j) {
              pos[NDim - 1 - j] = (i / acc) % memorySizes[NDim - 1 - j];
              acc *= memorySizes[NDim - 1 - j];
            }
            device::apply([&](const auto... idx) { mem[i] = coordinateToValue(pos); }, pos);
          });

      // order == 0: r2c then c2r. Order == 1: c2r then r2c.
      sayMPI << "About to do FFT.\n";
      if (!order) ffter.r2c(mem);
      ffter.c2r(mem);
      if (order) ffter.r2c(mem);
      sayMPI << "Finished FFT.\n";

      // auto emptyPadding = [&](auto pos, auto vPos) {
      //     if ( vPos.back() >= memorySizes.back() - 2) {
      // std::cerr << "Padding " << vPos << " " << mem[pos] << "\n";
      //         mem[pos] = 100000 ;
      //     }
      //     return true;
      // };
      //  verified: no effect on c2r - r2c result;
      // if ( ! order ) iterate(emptyPadding, 0, 0, actualVPos);

      const T norm = 1. / std::pow(nGrid[0], NDim);

      bool allRight = true;
      auto checkMem = [&](const auto val, const auto vPos) {
        // sayMPI << "checkMem " << pos << "\n";
        T valueShouldBe = coordinateToValue(vPos);

        if ((!order) && vPos[NDim - 1] >= memorySizes[NDim - 1] - 2) return true;

        bool thisCheck = AlmostEqual(valueShouldBe, norm * val, std::is_same<T, float>::value ? 0.1 : 1e-4);

        allRight = allRight && thisCheck;
        //            if ( order && allRight ) say << "Equal: " << vPos << " -> " << valueShouldBe << " == " << norm *
        //            mem[pos] << "\n";
        if (!thisCheck) {
          say << "Not equal: " << vPos << " -> " << valueShouldBe << " != " << norm * val << " = " << norm << " * "
              << val << "\n";
          //                for ( int ii = 1; ii < 20; ++ii)
          //                    std::cerr << "pos + " << ii << " = " << (pos + ii) << ": " << norm * mem[pos + ii] << "
          //                    = " << norm << " * " << mem[pos + ii] << "\n";
          //                std::cerr << "\n\n";
        }
        return allRight;
      };

      auto result_view = mem.getRawHostView();
      for (size_t i = 0; i < mem.size(); ++i) {
        device::array<ptrdiff_t, NDim> pos{};
        size_t acc = 1;
        for (size_t j = 0; j < NDim; ++j) {
          pos[NDim - 1 - j] = (i / acc) % memorySizes[NDim - 1 - j];
          acc *= memorySizes[NDim - 1 - j];
        }
        checkMem(result_view(i), pos);
      }

      bool r2c_then_c2r = allRight;
      bool c2r_then_r2c = allRight;
      if (order)
        tdd.verify(c2r_then_r2c);
      else
        tdd.verify(r2c_then_c2r);
      if (!allRight)
        sayMPI << "Failed for NDim: " << NDim << ", nGrid: " << nGrid << "\n";
      else
        sayMPI << "Success for NDim: " << NDim << ", nGrid: " << nGrid << "\n";
    }
  };

  for (ptrdiff_t inGrid = 1; inGrid < 6; ++inGrid) {
    device::array<ptrdiff_t, 2> nGrid;
    for (auto &it : nGrid)
      it = std::pow(2, inGrid);
    myLittleLambda(nGrid);
  }
  for (ptrdiff_t inGrid = 1; inGrid < 5; ++inGrid) {
    device::array<ptrdiff_t, 3> nGrid;
    for (auto &it : nGrid)
      it = std::pow(2, inGrid);
    myLittleLambda(nGrid);
  }
  for (ptrdiff_t inGrid = 1; inGrid < 4; ++inGrid) {
    device::array<ptrdiff_t, 4> nGrid;
    for (auto &it : nGrid)
      it = std::pow(2, inGrid);
    myLittleLambda(nGrid);
  }
}

template <size_t NDim> inline void TempLat::FFTLibrarySelector<NDim>::Test(TempLat::TDDAssertion &tdd)
{
#ifndef NOFFTFLOAT
  TestBody<float>(tdd);
#endif
  TestBody<double>(tdd);
}

#endif
