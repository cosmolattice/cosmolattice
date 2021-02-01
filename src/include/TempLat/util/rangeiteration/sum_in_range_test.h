#ifndef TEMPLAT_UTIL_RANGEITERATION_SUM_IN_RANGE_TEST_H
#define TEMPLAT_UTIL_RANGEITERATION_SUM_IN_RANGE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::sum_in_range_Tester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( sum_in_range<1,26>([](auto i){return i;}) == (25+1)*25 /2 );

}

#endif
