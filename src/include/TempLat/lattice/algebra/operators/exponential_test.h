#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_EXPONENTIAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_EXPONENTIAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

//#include "TempLat/lattice/algebra/operators/divide.h"

inline void TempLat::ExponentialTester::Test(TempLat::TDDAssertion& tdd) {

  class myClass{
  public:
    myClass(int b):a(b){}

    auto get(const double& i)
    {
      return a;
    }

  private:
    double a;
  };

    myClass a(3);
    //myClass b(4);
    say << exp(a).get(0) << "\n";
    tdd.verify( AlmostEqual(exp(a).get(0),std::exp(3))  );
//    say << exp(a / b).d(a) << "\n";
//    say << exp(a / b).d(b) << "\n";
//
//    /* Default is to fail: to remind yourself to implement something here. */

}

#endif
