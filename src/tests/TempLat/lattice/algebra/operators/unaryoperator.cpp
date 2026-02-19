
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

// namespace {
//     TempLat::TDDContainer<TempLat::UnaryOperatorTester> test;
// }


namespace TempLat {

template<typename T>
struct UnaryOperatorTester {
  static void Test(TDDAssertion &tdd);
};

template <typename T> inline void UnaryOperatorTester<T>::Test(TDDAssertion &tdd) { tdd.verify(true); }

} // namespace TempLat
