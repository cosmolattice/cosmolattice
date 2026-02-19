
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/exception.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ExceptionTester {
    static void Test(TDDAssertion &tdd);
  };

  MakeException(TestException);

  void ExceptionTester::Test(TDDAssertion &tdd)
  {

    tdd.verify(Throws<TestException>([] { throw TestException("Hoi!"); }));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ExceptionTester> test;
}
