#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasEvalTester::Test(TempLat::TDDAssertion &tdd)
{
  struct myTest {
    KOKKOS_FORCEINLINE_FUNCTION
    void eval(ptrdiff_t i) { m = 76; }
    double m;
  };
  struct myTest2 {
    KOKKOS_FORCEINLINE_FUNCTION
    double eval() { return 777; }
  };

  tdd.verify(HasEval<myTest> == true);
  tdd.verify(HasEval<myTest2> == false);
  tdd.verify(HasEval<double> == false);
}

#endif
