
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2020

#include "CosmoInterface/couplingsmanager.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct CouplingsManagerTester {
    static void Test(TDDAssertion &tdd);
  };

  void CouplingsManagerTester::Test(TDDAssertion &tdd)
  {
    CouplingsManager<2, 4, true, false, true, false, true, true, true, false> cm1;

    // static assert instead of usual test to be sure everything is accessible at compile time.

    static_assert(cm1.couples(0_c, 0_c) == true, "Should be accessible at compile time");
    static_assert(cm1.couples(0_c, 1_c) == false, "Should be accessible at compile time");
    static_assert(cm1.couples(0_c, 2_c) == true, "Should be accessible at compile time");
    static_assert(cm1.couples(0_c, 3_c) == false, "Should be accessible at compile time");
    static_assert(cm1.couples(1_c, 0_c) == true, "Should be accessible at compile time");
    static_assert(cm1.couples(1_c, 1_c) == true, "Should be accessible at compile time");
    static_assert(cm1.couples(1_c, 2_c) == true, "Should be accessible at compile time");
    static_assert(cm1.couples(1_c, 3_c) == false, "Should be accessible at compile time");
    static_assert(cm1.howManyCouples() == 5, "Should be accessible at compile time");

    cm1.setEffectiveCharges({1, 2, 3, 4, 5}, {1, 2, 3, 4});

    tdd.verify(cm1(0_c, 0_c) == 1);
    tdd.verify(cm1(0_c, 2_c) == 6);
    tdd.verify(cm1(1_c, 0_c) == 3);
    tdd.verify(cm1(1_c, 1_c) == 8);
    tdd.verify(cm1(1_c, 2_c) == 15);

    // should and does abort.
    // cm1.setCharges({1,1,2,3,4,5});
    // cm1.setCharges({1,3,4,5});
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::CouplingsManagerTester> test;
}
