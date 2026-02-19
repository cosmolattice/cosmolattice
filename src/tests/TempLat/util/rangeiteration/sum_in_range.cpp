
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct sum_in_range_Tester {
    static void Test(TDDAssertion &tdd);
  };

  void sum_in_range_Tester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(sum_in_range<1, 26>([](auto i) { return i; }) == (25 + 1) * 25 / 2);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::sum_in_range_Tester> test;
}
