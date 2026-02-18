#ifndef TEMPLAT_UTIL_STATICIF_TEST_H
#define TEMPLAT_UTIL_STATICIF_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/constants/zerotype.h"

inline void TempLat::StaticIfTester::Test(TempLat::TDDAssertion &tdd)
{
  // IfElse: true branch
  tdd.verify(IfElse(true, true, false));
  // IfElse: false branch
  tdd.verify(!IfElse(false, true, false));
  // IfElse: different types
  tdd.verify(IfElse(true, 42, 0) == 42);
  tdd.verify(IfElse(false, 42, 0) == 0);
}

#endif
