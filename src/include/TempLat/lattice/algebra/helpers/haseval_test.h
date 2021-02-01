#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasEvalTester::Test(TempLat::TDDAssertion& tdd) {

    struct myTest{
        void eval(ptrdiff_t i){
            m = 76;
        }
        double m;
    };
    struct myTest2{
        double eval(){
            return 777;
        }
    };
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( HasEval<myTest>::value == true);
    tdd.verify( HasEval<myTest2>::value == false);
    tdd.verify( HasEval<double>::value == false);

}

#endif
