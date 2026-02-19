
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/memory/triplestatelayouts.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <size_t NDim> struct TripleStateLayoutsTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void TripleStateLayoutsTester<NDim>::Test(TDDAssertion &tdd)
  {
    FFTLibrarySelector<4> theLibrary(MPICartesianGroup(4, {{MPICommReference().size(), 1, 1, 1}}),
                                     {256, 256, 256, 256});

    TripleStateLayouts<4> tsl(theLibrary.getLayout(), 10);

    if (TDDRegister::isSingleUnitTest()) {
      say << tsl << "\n";
    }

    /* compute the expected memory size: */
    ptrdiff_t d1Local = 256 / (ptrdiff_t)MPICommReference().size();

    ptrdiff_t expectedMem = (d1Local + 2 * 10) * std::pow(256 + 2 * 10, 4 - 1);

    tdd.verify(tsl.getNecessaryMemoryAllocation() == expectedMem);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TripleStateLayoutsTester<3>> test;
}
