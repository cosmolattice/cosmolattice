
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/memory/memorylayouts/hermitianvalueaccounting.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct HermitianValueAccountingTester {
    static void Test(TDDAssertion &tdd);
  };

  void HermitianValueAccountingTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    bool allTestsAreInRandomFieldAndVerifyHermitianLayout = true;
    tdd.verify(allTestsAreInRandomFieldAndVerifyHermitianLayout);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HermitianValueAccountingTester> test;
}
