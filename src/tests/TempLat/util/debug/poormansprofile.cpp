
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/debug/poormansprofile.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

struct PoorMansProfileTester {
  static void Test(TDDAssertion &tdd);
};

void PoorMansProfileTester::Test(TDDAssertion &tdd)
{

  if (TDDRegister::isSingleUnitTest()) {

    auto &&f1 = []() { PoorMansProfile::TrackMe(); };
    auto &&f2 = []() { PoorMansProfile::TrackMe(); };
    for (int i = 0; i < 100; ++i) {
      f1();
      if (i % 3 == 0) f2();
    }
  }
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::PoorMansProfileTester> test;
}
