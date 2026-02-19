
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/fft/external/fftw/fftwmemorylayout.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/fftw/fftwinterface.h"

namespace TempLat
{

  struct FFTWMemoryLayoutTester {
    static void Test(TDDAssertion &tdd)
    {
      MPICommReference world;

      //    FFTWMemoryLayout mem; // cannot do this. Need the descending class for that.

      auto computeExpectation = [&](const auto &nGrid, const auto tag) {
        constexpr size_t nDim = decltype(tag)::value;
        FFTWInterface<nDim> mem;

        FFTLayoutStruct<nDim> expected(nGrid);
        device::IdxArray<nDim> confLocalSizes(nGrid);
        device::IdxArray<nDim> confLocalStarts{};
        device::IdxArray<nDim> fourLocalSizes(nGrid);
        device::IdxArray<nDim> fourLocalStarts{};
        device::IdxArray<nDim> fourTransposition{};
        device::array<device::IdxArray<2>, nDim> confPadding{};
        for (size_t i = 0; i < nDim; ++i)
          fourTransposition[i] = i;

        fourLocalSizes[nDim - 1] = fourLocalSizes[nDim - 1] / 2 + 1;
        confPadding[nDim - 1][1] = 2;

        expected.fourierSpace = LayoutStruct<nDim>::createGlobalFFTLayout(nGrid);

        if (nDim > 1) {
          confLocalStarts[0] = (nGrid[0] * (ptrdiff_t)world.rank()) / (ptrdiff_t)world.size();
          confLocalSizes[0] /= world.size();
          fourLocalStarts[0] = (nGrid[0] * (ptrdiff_t)world.rank()) / (ptrdiff_t)world.size();
          fourLocalSizes[0] /= world.size();
        }

        if (nDim > 2 && world.size() > 1) {
          std::swap(fourLocalStarts[0], fourLocalStarts[1]);
          std::swap(fourLocalSizes[0], fourLocalSizes[1]);
          std::swap(fourTransposition[0], fourTransposition[1]);
        }

        expected.fourierSpace.setLocalSizes(fourLocalSizes);
        expected.fourierSpace.setLocalStarts(fourLocalStarts);
        expected.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);
        expected.configurationSpace.setLocalSizes(confLocalSizes);
        expected.configurationSpace.setLocalStarts(confLocalStarts);
        expected.configurationSpace.setPadding(confPadding);

        return expected;
      };

      //    say << mem.computeLocalSizes(MPICartesianGroup(2), 4, 128) << "\nexpectation" << computeExpectation(4, 128);

      ptrdiff_t wSize = world.size();
      if (128 % wSize) {
        bool test_Only_Works_For_World_Size_Which_Is_A_Factor_Of_128 = false;
        tdd.verify(test_Only_Works_For_World_Size_Which_Is_A_Factor_Of_128);
      } else {

        auto &&doSingleTest = [&](const auto &nGrid, const auto tag) {
          constexpr size_t nDim = decltype(tag)::value;
          std::vector<int> decompose(nDim, 1);
          decompose[0] = world.size();
          FFTWInterface<nDim> mem;
          auto fromLibrary = mem.computeLocalSizes(MPICartesianGroup(nDim, decompose), nGrid);
          auto expected = computeExpectation(nGrid, tag);
          bool result = fromLibrary == expected;
          tdd.verify(fromLibrary == expected);
          if (!result) {
            say << "Failure for layouts, from library:\n" << fromLibrary << "\bexpected:\n" << expected << "\n\n";
          }
        };

        doSingleTest(device::IdxArray<2>{128, 128}, Tag<2>{});
        doSingleTest(device::IdxArray<4>{128, 128, 128, 128}, Tag<4>{});
        doSingleTest(device::IdxArray<6>{128, 128, 128, 128, 128, 128}, Tag<6>{});
      }
    }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FFTWMemoryLayoutTester> test;
}
