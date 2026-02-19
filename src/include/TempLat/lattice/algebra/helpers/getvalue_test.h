#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tagliteral.h"

template <typename T> inline void TempLat::GetValueTester::Test(T &tdd)
{
  tdd.verify(GetValue::get(Tag<1>(), 0, 1) == 1);
  tdd.verify(GetValue::get(Tag<1>()) == 1);
  tdd.verify(GetValue::get(Tag<7>(), 3, 4, 5) == 7);
  tdd.verify(GetValue::get(Tag<7>()) == 7);
}

#endif
