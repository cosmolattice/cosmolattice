
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

// namespace {
//     TempLat::TDDContainer<TempLat::BinaryOperatorTester> test;
// }


namespace TempLat {

struct BinaryOperatorTester {
  static void Test(TDDAssertion &tdd);
};

// void BinaryOperatorTester::Test(TDDAssertion& tdd) {
//
//     tdd.verify( true );
//
// }

} // namespace TempLat
