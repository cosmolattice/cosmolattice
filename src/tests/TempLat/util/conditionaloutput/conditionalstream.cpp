
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/conditionaloutput/conditionalstream.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ConditionalStreamTester {
    static void Test(TDDAssertion &tdd);
  };

  void ConditionalStreamTester::Test(TDDAssertion &tdd)
  {

    std::stringstream str1, str2;
    ConditionalStream one(str1, true);
    ConditionalStream two(str2, false);

    one << "Hoi!";
    two << "Doei!";

    tdd.verify(str1.str().length() > 0);
    tdd.verify(str2.str().length() == 0);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ConditionalStreamTester> test;
}
