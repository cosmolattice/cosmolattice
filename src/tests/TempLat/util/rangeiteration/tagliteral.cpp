
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct TagLiteralTester {
    static void Test(TDDAssertion &tdd);
  };

  void TagLiteralTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    auto myLambda = [](Tag<9> t) { return 9; };
    ;
    tdd.verify(9 == myLambda(9_c));
    // myLambda(9); does not compile as it should.
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TagLiteralTester> test;
}
