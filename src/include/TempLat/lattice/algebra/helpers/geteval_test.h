#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::GetEval::Test(TempLat::TDDAssertion &tdd)
{
  struct myTest {
    KOKKOS_FORCEINLINE_FUNCTION
    double getEval(ptrdiff_t i) { return 76; }

    KOKKOS_FORCEINLINE_FUNCTION
    double get(ptrdiff_t i) { return 63; }
  };
  struct myTest2 {
    KOKKOS_FORCEINLINE_FUNCTION
    double get(ptrdiff_t i) { return 777; }
  };

  myTest t1;
  myTest2 t2;

  tdd.verify(GetEval::getEval(t1, 8) == 76);
  tdd.verify(GetEval::getEval(t2, 8) == 777);
  tdd.verify(GetEval::getEval(23, 8) == 23);
}

#endif
