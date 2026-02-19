
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/listoperators/total.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct TotalTester {
    static void Test(TDDAssertion &tdd);
  };

  void TotalTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    auto test = std::make_tuple(1, 2, 3, 4);

    tdd.verify(total(test) == 10);
    tdd.verify(total(test, [](auto x) { return 2 * x; }) == 20);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TotalTester> test;
}
