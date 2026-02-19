
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/initializers/u1initializer.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

struct U1InitializerTester {
  static void Test(TDDAssertion &tdd);
};

void U1InitializerTester::Test(TDDAssertion &tdd)
{

  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(true);
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::U1InitializerTester> test;
}
