#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISTempLatGETTABLE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISTempLatGETTABLE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019


inline void TempLat::IsTempLatGettableTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    struct myDummyStruct{

        int getComp(Tag<0> t){return 24;}
    };
    struct myDummyStruct2{

        int get(int t){return 24;}
    };
    tdd.verify( IsTempLatGettable<0,std::tuple<int,int>>::value == false );
    tdd.verify( IsTempLatGettable<0,myDummyStruct>::value == true );
    tdd.verify( IsTempLatGettable<0,myDummyStruct2>::value == false );


}

#endif
