#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_VARIADICINDEX_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_VARIADICINDEX_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

template <> inline void TempLat::VariadicIndexTester::Test<TempLat::TDDAssertion>(TempLat::TDDAssertion &tdd)
{
  struct some_type {
  };

  tdd.verify(VariadicIndex<size_t, const size_t &, size_t &&> == true);
  tdd.verify(VariadicIndex<size_t, const size_t &, size_t &&, int> == true);
  tdd.verify(VariadicIndex<some_type> == false);
  tdd.verify(VariadicIndex<size_t, const size_t &, size_t &&, double &&> == false);

  tdd.verify(VariadicNDIndex<3, size_t, const size_t &, size_t &&> == true);
  tdd.verify(VariadicNDIndex<4, size_t, const size_t &, size_t &&> == false);
  tdd.verify(VariadicNDIndex<4, size_t, const size_t &, size_t &&, int> == true);
  tdd.verify(VariadicNDIndex<2, size_t, const size_t &, size_t &&, int> == false);
  tdd.verify(VariadicNDIndex<1, some_type> == false);
  tdd.verify(VariadicNDIndex<4, size_t, const size_t &, size_t &&, double &&> == false);
}

#endif
