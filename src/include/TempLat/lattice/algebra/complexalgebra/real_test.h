#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::RealTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyStruct{
        int ComplexFieldGet(Tag<0> t){
            return 87;
        };
        double ComplexFieldGet(Tag<1> t){
            return 870;
        };
    };


    MyStruct ms;
    tdd.verify( Real(ms) == 87 );

}

#endif
