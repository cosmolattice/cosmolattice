
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "CosmoInterface/abstractmodel.h"
#include "TempLat/util/tdd/tdd.h"

// namespace {
//     TempLat::TDDContainer<TempLat::AbstractModelTester> test;
// }

namespace TempLat
{

  struct AbstractModelTester {
    static void Test(TDDAssertion &tdd);
  };

  // void AbstractModelTester::Test(TDDAssertion& tdd) {
  //
  //     /* Default is to fail: to remind yourself to implement something here. */
  //     tdd.verify( false );
  //
  // }

} // namespace TempLat
