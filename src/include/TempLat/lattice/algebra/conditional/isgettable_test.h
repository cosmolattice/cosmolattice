#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_ISGETTABLE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_ISGETTABLE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

//#include "TempLat/lattice/algebra/gettergetoffset.h"

inline void TempLat::IsGettableTester::Test(TempLat::TDDAssertion& tdd) {

  class MyClass{
  public:
    MyClass(int b):a(b){}

    auto get(ptrdiff_t i)
    {
      return a;
    }

  private:
    int a;
  };
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( IsGettable<MyClass>::value );
    tdd.verify( ! IsGettable<double>::value );

}

#endif
