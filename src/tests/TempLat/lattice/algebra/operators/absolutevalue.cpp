
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/absolutevalue.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct AbsoluteValueTester {
    static void Test(TDDAssertion &tdd);
  };

  void AbsoluteValueTester::Test(TDDAssertion &tdd)
  {
    struct tmpStruct {
      DEVICE_FORCEINLINE_FUNCTION
      ptrdiff_t get(ptrdiff_t) const { return -1; }
      DEVICE_FORCEINLINE_FUNCTION
      ptrdiff_t eval(ptrdiff_t) const { return -1; }
    };
    struct tmpStruct2 {
      DEVICE_FORCEINLINE_FUNCTION
      complex<double> get(ptrdiff_t) const { return complex<double>(1, 1); }
      DEVICE_FORCEINLINE_FUNCTION
      complex<double> eval(ptrdiff_t) const { return complex<double>(1, 1); }
    };
    tdd.verify(abs(tmpStruct()).eval(0) == 1);
    tdd.verify(abs(tmpStruct2()).eval(0) == std::sqrt(2));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::AbsoluteValueTester> test;
}
