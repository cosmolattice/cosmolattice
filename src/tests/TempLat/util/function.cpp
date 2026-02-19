
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/util/function.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct FunctionTester {
    static void Test(TDDAssertion &tdd);
  };

  void FunctionTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(Function(x, 2 * x)(2) == 4);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FunctionTester> test;
}
