
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/staticif.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"

namespace TempLat
{

  struct StaticIfTester {
    static void Test(TDDAssertion &tdd);
  };

  void StaticIfTester::Test(TDDAssertion &tdd)
  {
    // IfElse: true branch
    tdd.verify(IfElse(true, true, false));
    // IfElse: false branch
    tdd.verify(!IfElse(false, true, false));
    // IfElse: different types
    tdd.verify(IfElse(true, 42, 0) == 42);
    tdd.verify(IfElse(false, 42, 0) == 0);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::StaticIfTester> test;
}
