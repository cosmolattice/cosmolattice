
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/complexalgebra/complexfieldconjugate.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ComplexFieldConjugateTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexFieldConjugateTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    struct MyStruct {
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<0> t) const { return 1; }
      DEVICE_FORCEINLINE_FUNCTION
      int ComplexFieldGet(Tag<1> t) const { return 2; }
    };
    tdd.verify(Real(conj(MyStruct())) == 1);
    tdd.verify(Imag(conj(MyStruct())) == -2);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexFieldConjugateTester> test;
}
