
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/tuple_size_tester.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tuple_size.h"

namespace TempLat
{

  struct testtuple {
    static const size_t size = 42;
  };

  struct tuple_size_Tester {
    static void Test(TDDAssertion &tdd);
  };

  void tuple_size_Tester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(tuple_size<testtuple>::value == 42);
    tdd.verify(tuple_size<int>::value == 1);
    tdd.verify(tuple_size<std::tuple<int, int, int>>::value == 3);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::tuple_size_Tester> test;
}
