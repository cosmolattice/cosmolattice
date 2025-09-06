#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ABSOLUTEVALUE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ABSOLUTEVALUE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::AbsoluteValueTester::Test(TempLat::TDDAssertion &tdd)
{
  struct tmpStruct {
    DEVICE_FORCEINLINE_FUNCTION
    ptrdiff_t get(ptrdiff_t) const { return -1; }
  };
  struct tmpStruct2 {
    DEVICE_FORCEINLINE_FUNCTION
    complex<double> get(ptrdiff_t) const { return complex<double>(1, 1); }
  };
  tdd.verify(abs(tmpStruct()).get(0) == 1);
  tdd.verify(abs(tmpStruct2()).get(0) == std::sqrt(2));
}

#endif
