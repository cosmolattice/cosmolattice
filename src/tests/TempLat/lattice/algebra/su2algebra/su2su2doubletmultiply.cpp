
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2su2doubletmultiply.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SU2SU2DoubletMultiplyTester {
    static void Test(TDDAssertion &tdd);
  };

  void SU2SU2DoubletMultiplyTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SU2SU2DoubletMultiplyTester> test;
}
