
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/complexalgebra/complexfieldsubtract.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ComplexFieldSubtractTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexFieldSubtractTester::Test(TDDAssertion &tdd)
  {
    struct MyStruct {
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<0> t) const { return 1; }
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<1> t) const { return 2; }
      using Getter [[maybe_unused]] = ComplexFieldGetter;
    };
    struct MyStruct2 {
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<0> t) const { return 3; }
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<1> t) const { return 4; }

      using Getter [[maybe_unused]] = ComplexFieldGetter;
    };

    auto test = MyStruct() - MyStruct2();
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(test.ComplexFieldGet(0_c) == -2);
    tdd.verify(test.ComplexFieldGet(1_c) == -2);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexFieldSubtractTester> test;
}
