
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct HasSU2GetTester {
    static void Test(TDDAssertion &tdd);
  };

  void HasSU2GetTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    struct MyStruct {
      DEVICE_FORCEINLINE_FUNCTION
      double SU2Get(Tag<0> t) { return 87; };
    };
    struct MyStruct2 {
      DEVICE_FORCEINLINE_FUNCTION
      double getComp(Tag<0> t) { return 87; };
    };

    tdd.verify(HasSU2Get<MyStruct> == true);
    tdd.verify(HasSU2Get<MyStruct2> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasSU2GetTester> test;
}
