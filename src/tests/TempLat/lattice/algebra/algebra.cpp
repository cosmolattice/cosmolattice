
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
// #include "TempLat/lattice/algebra/algebra.h"

/** \file algebra.h is a collection header, no class, no test. */

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

struct Algebra {
  static void Test(TDDAssertion &tdd);
};

void Algebra::Test(TDDAssertion &tdd) { tdd.verify(true); }

} // namespace TempLat
