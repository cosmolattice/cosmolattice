
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/listoperators/listadd.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/foreach.h"

namespace TempLat
{

  struct ListAddTester {
    static void Test(TDDAssertion &tdd);
  };

  void ListAddTester::Test(TDDAssertion &tdd)
  {

    auto t1 = std::make_tuple(1, 2, 3, 4);
    auto t2 = std::make_tuple(5, 6, 7, 8);

    auto t3 = t1 + t2;

    int tmp = 6;
    for_each(t3, [&](auto x) {
      tdd.verify(tmp == x);
      tmp += 2;
    });
    auto t4 = t1 + 9;
    tdd.verify(GetComponent::get(t4, Tag<0>()) == 10);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ListAddTester> test;
}
