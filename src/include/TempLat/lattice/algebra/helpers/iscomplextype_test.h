#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::IsComplexTypeTester::Test(TempLat::TDDAssertion &tdd)
{
  tdd.verify(IsComplexType<complex<float>>);
  tdd.verify(IsComplexType<complex<double>>);
  tdd.verify(IsComplexType<const complex<float>>);
  tdd.verify(IsComplexType<const complex<double>>);
  tdd.verify(IsComplexType<const complex<float> &>);
  tdd.verify(IsComplexType<const complex<double> &>);

  tdd.verify(!IsComplexType<int>);
  tdd.verify(!IsComplexType<std::array<int, 3>>);
  tdd.verify(!IsComplexType<float>);
  tdd.verify(!IsComplexType<double>);
  tdd.verify(!IsComplexType<float &>);
  tdd.verify(!IsComplexType<double &>);
}

#endif
