
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/measuringtools/projectionhelpers/radialbincomputer.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct RadialBinComputerTester {
    static void Test(TDDAssertion &tdd);
  };

  void RadialBinComputerTester::Test(TDDAssertion &tdd)
  {

    RadialBinComputer pc(1, 3, 4);

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(pc(1.1) == 0);
    tdd.verify(pc(0) == 0);

    tdd.verify(pc(2.1) == 2);
    tdd.verify(pc(5) == 3);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RadialBinComputerTester> test;
}
