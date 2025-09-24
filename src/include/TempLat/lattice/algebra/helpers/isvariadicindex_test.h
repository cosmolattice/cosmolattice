#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_IsVariadicIndex_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_IsVariadicIndex_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

template <> inline void TempLat::IsVariadicIndexTester::Test<TempLat::TDDAssertion>(TempLat::TDDAssertion &tdd)
{
  struct some_type {
  };

  tdd.verify(IsVariadicIndex<size_t, const size_t &, size_t &&> == true);
  tdd.verify(IsVariadicIndex<size_t, const size_t &, size_t &&, int> == true);
  tdd.verify(IsVariadicIndex<some_type> == false);
  tdd.verify(IsVariadicIndex<size_t, const size_t &, size_t &&, double &&> == false);

  tdd.verify(IsVariadicNDIndex<3, size_t, const size_t &, size_t &&> == true);
  tdd.verify(IsVariadicNDIndex<4, size_t, const size_t &, size_t &&> == false);
  tdd.verify(IsVariadicNDIndex<4, size_t, const size_t &, size_t &&, int> == true);
  tdd.verify(IsVariadicNDIndex<2, size_t, const size_t &, size_t &&, int> == false);
  tdd.verify(IsVariadicNDIndex<1, some_type> == false);
  tdd.verify(IsVariadicNDIndex<4, size_t, const size_t &, size_t &&, double &&> == false);
}

#endif
