
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/staticwarning.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

struct StaticWarningTester {
  static void Test(TDDAssertion &tdd);
};

void StaticWarningTester::Test(TDDAssertion &tdd)
{

  // uncomment if you are actually testing this thing. Comment otherwise, because you would see a warning every time
  // this header is included somewhere.
  //    static_warn(false, "You must see this at compilation.");

  static_warn(true, "You must NEVER see this at compilation.");
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::StaticWarningTester> test;
}
