#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_HASCOMPLEXFIELDGET_TEST_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_HASCOMPLEXFIELDGET_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasComplexFieldGetTester::Test(TempLat::TDDAssertion& tdd) {


    struct MyStruct{
        double ComplexFieldGet(Tag<0> t){
            return 87;
        };
    };
    struct MyStruct2{
        double getComp(Tag<0> t){
            return 87;
        };
    };

    tdd.verify( HasComplexFieldGet<MyStruct>::value == true );
    tdd.verify( HasComplexFieldGet<MyStruct2>::value == false );

}

#endif
