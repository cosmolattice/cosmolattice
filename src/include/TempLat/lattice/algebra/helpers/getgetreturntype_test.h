#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

template <typename T> inline void TempLat::GetGetReturnTypeTester::Test(T &tdd)
{
  struct tmp {
    DEVICE_FORCEINLINE_FUNCTION
    double get(const ptrdiff_t &) const { return 0.; }
  };

  struct tmp2 {
    DEVICE_FORCEINLINE_FUNCTION
    complex<double> get(const ptrdiff_t &) const { return complex<double>(0., 0.); }
  };

  //  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(std::is_same_v<typename GetGetReturnType<tmp>::type, double>);
  tdd.verify(!GetGetReturnType<tmp>::isComplex);

  tdd.verify(!std::is_same_v<typename GetGetReturnType<tmp2>::type, double>);
  tdd.verify(std::is_same_v<typename GetGetReturnType<tmp2>::type, complex<double>>);
  tdd.verify(GetGetReturnType<tmp2>::isComplex);
}

#endif
