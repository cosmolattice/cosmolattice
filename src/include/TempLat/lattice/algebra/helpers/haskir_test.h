#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASKIR_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASKIR_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasKIRTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    struct myTest{
      double getKIR(){
        return 76;
      }
    };
    struct myTest2{
      double getdkkhdiu(){
        return 777;
      }
    };
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( HasKIR<myTest>::value == true);
    tdd.verify( HasKIR<myTest2>::value == false);
}

#endif
