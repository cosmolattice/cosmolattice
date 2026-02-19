
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/tuplemaker.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

struct TupleMakerNotTester {
  static void Test(TDDAssertion &tdd);
};

void TupleMakerNotTester::Test(TDDAssertion &tdd)
{
  /* Default is to fail: to remind yourself to implement something here. */
  // test in tuplemaketester_test.h
  tdd.verify(true);
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TupleMakerNotTester> test;
}
