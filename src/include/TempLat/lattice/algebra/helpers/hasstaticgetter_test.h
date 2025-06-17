#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"

inline void TempLat::HasStaticGetterDummy::Test(TempLat::TDDAssertion &tdd)
{
  auto test = Complexify(1, 2);

  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(TypeHasStaticGet<decltype(test)> == true);
  tdd.verify(TypeHasStaticGet<double> == false);
  tdd.verify(TypeHasStaticGet<Field<3, double>> == false);
}

#endif
