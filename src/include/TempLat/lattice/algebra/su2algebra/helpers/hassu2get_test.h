#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2GET_TEST_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2GET_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasSU2GetTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */


    struct MyStruct{
        double SU2Get(Tag<0> t){
          return 87;
        };
    };
    struct MyStruct2{
        double getComp(Tag<0> t){
            return 87;
        };
    };

    tdd.verify( HasSU2Get<MyStruct>::value == true );
    tdd.verify( HasSU2Get<MyStruct2>::value == false );

}

#endif
