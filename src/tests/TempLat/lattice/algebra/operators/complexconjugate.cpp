
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/complexconjugate.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ComplexConjugateTester {
    static void Test(TDDAssertion &tdd);
  };

  void ComplexConjugateTester::Test(TDDAssertion &tdd)
  {

    struct myStruct {
      DEVICE_FORCEINLINE_FUNCTION
      complex<double> get(ptrdiff_t i) const { return complex<double>(1, -1); }
      DEVICE_FORCEINLINE_FUNCTION
      complex<double> eval(ptrdiff_t i) const { return complex<double>(1, -1); }
    };

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(myStruct().eval(0) == complex<double>(1, -1));

    tdd.verify(conj(myStruct()).eval(0) == complex<double>(1, 1));

    /* test: this should not compile. Passed. */
    //    struct myFailStruct {
    //        double get(IterationCoordinates&) {
    //            return 1;
    //        }
    //    };
    //
    //    tdd.verify( conj(myFailStruct()).get(pIterCoords) == complex<double>(1, 1) );
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ComplexConjugateTester> test;
}
