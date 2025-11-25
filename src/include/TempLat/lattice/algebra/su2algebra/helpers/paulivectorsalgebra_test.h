#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_HELPERS_PAULIVECTORSALGEBRA_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_HELPERS_PAULIVECTORSALGEBRA_TEST_H
#include "TempLat/util/almostequal.h"

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

inline void TempLat::PauliVectorsAlgebra::Test(TempLat::TDDAssertion &tdd)
{
  /* Default is to fail: to remind yourself to implement something here. */

  std::array<double, 4> A = {2.4, 0.96, 0.1, 0.01};
  std::array<double, 4> B = {2.4, 0.56, 3.4, 0.01};
  std::array<double, 4> c;

  PauliVectorsAlgebra::multiply_inplace(c, A, B);

  tdd.verify(AlmostEqual(c[0], 4.8823));
  tdd.verify(AlmostEqual(c[1], 3.681));
  tdd.verify(AlmostEqual(c[2], 8.404));
  tdd.verify(AlmostEqual(c[3], -3.16));
}

#endif
