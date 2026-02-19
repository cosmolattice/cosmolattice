
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/latinindiceslist.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct LatinIndicesListTester {
    static void Test(TDDAssertion &tdd);
  };

  void LatinIndicesListTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */

    auto test = make_latinindices_list(1, 9, 7);
    tdd.verify(test(1_c) == 1);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::LatinIndicesListTester> test;
}
