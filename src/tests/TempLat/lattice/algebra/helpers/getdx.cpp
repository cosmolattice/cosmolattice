
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct GetDxTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetDxTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    struct myTest {
      DEVICE_FORCEINLINE_FUNCTION
      double getDx() { return 0.42; }
    };
    struct myTest1 {
      DEVICE_FORCEINLINE_FUNCTION
      double getDx() { return 0.42; }

      DEVICE_FORCEINLINE_FUNCTION
      double get(ptrdiff_t i) { return 76; }
    };

    struct myTest2 {
      DEVICE_FORCEINLINE_FUNCTION
      double getI86R() { return 0.42; }

      DEVICE_FORCEINLINE_FUNCTION
      double get(ptrdiff_t i) { return 76; }
    };

    myTest1 t1;
    myTest2 t2;

    // myTest t;
    // tdd.verify(GetDx::getDx(t1) == 0.42); //does not compile as we don't have get method and is needed for automatic
    // type deducing.
    tdd.verify(GetDx::getDx(t1) == 0.42);
    tdd.verify(GetDx::getDx(t2) == 1);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetDxTester> test;
}
