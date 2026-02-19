/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/listoperators/listlog.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/foreach.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct ListLogTester {
    static void Test(TDDAssertion &tdd);
  };

  void ListLogTester::Test(TDDAssertion &tdd)
  {

    auto t1 = std::make_tuple(1., 2., 3., 4.);

    auto t3 = log(t1);
    double tmp = 1;

    for_each(t3, [&](auto x) { tdd.verify(AlmostEqual(x, std::log(tmp++))); });
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ListLogTester> test;
}
