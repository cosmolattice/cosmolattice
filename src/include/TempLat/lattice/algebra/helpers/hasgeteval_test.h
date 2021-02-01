#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETEVAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETEVAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasGetEvalTester::Test(TempLat::TDDAssertion& tdd) {


    struct myTest{
        double getEval(){
            return 76;
        }
    };
    struct myTest2{
        double getEval(ptrdiff_t i){
            return 777;
        }
    };
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( HasGetEval<myTest>::value == false);
    tdd.verify( HasGetEval<myTest2>::value == true);
    tdd.verify( HasGetEval<double>::value == false);

}

#endif
