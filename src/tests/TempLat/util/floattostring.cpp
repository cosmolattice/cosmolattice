
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/floattostring.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  struct FloatToStringTester {
    static void Test(TDDAssertion &tdd);
  };

  void FloatToStringTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(FloatToString::format(0.01) == std::string("0.01"));
    say << FloatToString::format(0.0001) << "\n";
    tdd.verify(FloatToString::format(0.0001) == std::string("1e-04"));
    say << FloatToString::format(1000.0001) << "\n";
    tdd.verify(FloatToString::format(1000.0001) == std::string("1e+03"));
  }
} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FloatToStringTester> test;
}
