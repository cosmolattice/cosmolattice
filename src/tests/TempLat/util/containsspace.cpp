
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/containsspace.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ContainsSpaceTester {
    static void Test(TDDAssertion &tdd);
  };

  void ContainsSpaceTester::Test(TDDAssertion &tdd)
  {

    tdd.verify(ContainsSpace::test("This has a space") == true);
    tdd.verify(ContainsSpace::test("Thishasnospace") == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ContainsSpaceTester> test;
}
