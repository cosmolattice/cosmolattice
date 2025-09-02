#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_TEST_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien,  Year: 2019

inline void TempLat::SU2WrapperTester::Test(TempLat::TDDAssertion &tdd)
{
  // Just to check if all compiles
  SU2Wrapper<double, double, double, double> w1(0., 0., 0., 0.);
  tdd.verify(w1.toString() == "SU2(0,0,0,0)");

  SU2Wrapper<double, double, double, double> w2(2.0, 3.0, 4.0, 4.0);
  tdd.verify(w2.toString() == "SU2(2,3,4,4)");
}

#endif
