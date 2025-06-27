#ifndef TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/constants/halftype.h"

class myClass
{
public:
  myClass(int b) : a(b) {}

  template <typename... IDX> KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...i) const { return a; }

private:
  double a;
};

inline void TempLat::UnaryMinusTester::Test(TempLat::TDDAssertion &tdd)
{
  myClass a(4);
  // myClass b(4);
  tdd.verify(AlmostEqual((-a).get(0), -4));
  tdd.verify(AlmostEqual((-HalfType()).get(0, 0, 0), -0.5));
}

#endif
