#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2GROUPWRAPPER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2GROUPWRAPPER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::SU2GroupWrapperTester::Test(TempLat::TDDAssertion &tdd)
{
  // Just to check if all compiles
  SU2GroupWrapper<double, double, double> w1(0., 0., 0.);
  tdd.verify(w1.toString() == "SU2Group(0,0,0)");

  SU2GroupWrapper<double, double, double> w2(2.0, 3.0, 4.0);
  tdd.verify(w2.toString() == "SU2Group(2,3,4)");
}

#endif
