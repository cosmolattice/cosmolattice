#ifndef TEMPLAT_UTIL_RANGEITERATION_TAGLITERAL_TEST_H
#define TEMPLAT_UTIL_RANGEITERATION_TAGLITERAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::TagLiteralTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    auto myLambda = [](Tag<9> t){ return 9;};
    ;
    tdd.verify( 9 == myLambda(9_c) );
    //myLambda(9); does not compile as it should.


}

#endif
