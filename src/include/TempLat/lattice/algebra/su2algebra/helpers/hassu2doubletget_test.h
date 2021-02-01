#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_TEST_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasSU2DoubletGetTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyStruct{
        double SU2DoubletGet(Tag<0> t){
            return 87;
        };
    };
    struct MyStruct2{
        double SU2Get(Tag<0> t){
            return 87;
        };
    };

    tdd.verify( HasSU2DoubletGet<MyStruct>::value == true );
    tdd.verify( HasSU2DoubletGet<MyStruct2>::value == false );

}

#endif
