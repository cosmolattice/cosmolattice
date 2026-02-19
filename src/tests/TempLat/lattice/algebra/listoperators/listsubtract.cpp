
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/listoperators/listsubtract.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/foreach.h"

namespace TempLat
{

  struct ListSubtractTester {
    static void Test(TDDAssertion &tdd);
  };

  void ListSubtractTester::Test(TDDAssertion &tdd)
  {

    auto t1 = std::make_tuple(1, 2, 3, 4);
    auto t2 = std::make_tuple(5, 6, 7, 8);

    auto t3 = t1 - t2;

    for_each(t3, [&](auto x) { tdd.verify(x == -4); });
    auto t4 = t1 - 9;
    tdd.verify(GetComponent::get(t4, Tag<0>()) == -8);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ListSubtractTester> test;
}
