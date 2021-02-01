#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDX_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDX_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasDxTester::Test(TempLat::TDDAssertion& tdd) {

    struct myTest{
      double getDx(){
        return 76;
      }
    };
    struct myTest2{
      double getdx(){
        return 777;
      }
    };
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( HasDx<myTest>::value == true);
    tdd.verify( HasDx<myTest2>::value == false);
    tdd.verify( HasDx<double>::value == false);

}

#endif
