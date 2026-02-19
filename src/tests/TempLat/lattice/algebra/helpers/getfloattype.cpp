
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct GetFloatTypeTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetFloatTypeTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(std::is_same<float, GetFloatType<float>::type>::value);

    tdd.verify(std::is_same<float, GetFloatType<complex<float>>::type>::value);

    tdd.verify(std::is_same<double, GetFloatType<double>::type>::value);

    tdd.verify(std::is_same<double, GetFloatType<complex<double>>::type>::value);

    tdd.verify(!std::is_same<double, GetFloatType<float>::type>::value);

    tdd.verify(!std::is_same<float, GetFloatType<complex<double>>::type>::value);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetFloatTypeTester> test;
}
