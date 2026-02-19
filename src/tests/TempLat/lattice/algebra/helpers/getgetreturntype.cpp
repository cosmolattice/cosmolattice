
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct GetGetReturnTypeTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetGetReturnTypeTester::Test(TDDAssertion &tdd)
  {
    struct tmp {
      DEVICE_FORCEINLINE_FUNCTION
      double eval(const ptrdiff_t &) const { return 0.; }
    };

    struct tmp2 {
      DEVICE_FORCEINLINE_FUNCTION
      complex<double> eval(const ptrdiff_t &) const { return complex<double>(0., 0.); }
    };

    //  /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(std::is_same_v<typename GetGetReturnType<tmp>::type, double>);
    tdd.verify(!GetGetReturnType<tmp>::isComplex);

    tdd.verify(!std::is_same_v<typename GetGetReturnType<tmp2>::type, double>);
    tdd.verify(std::is_same_v<typename GetGetReturnType<tmp2>::type, complex<double>>);
    tdd.verify(GetGetReturnType<tmp2>::isComplex);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetGetReturnTypeTester> test;
}
