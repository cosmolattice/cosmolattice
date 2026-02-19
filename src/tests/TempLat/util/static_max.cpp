
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/static_max.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct static_max_Tester {
    static void Test(TDDAssertion &tdd);
  };

  void static_max_Tester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(static_max<2, 3>::value == 3);
    tdd.verify(static_max<3, 2>::value == 3);
    tdd.verify(static_max<3, 3>::value == 3);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::static_max_Tester> test;
}
