#ifndef TEMPLAT_UTIL_STATIC_MAX_TEST_H
#define TEMPLAT_UTIL_STATIC_MAX_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::static_max_Tester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(static_max<2,3>::value == 3 );
    tdd.verify(static_max<3,2>::value == 3 );
    tdd.verify(static_max<3,3>::value == 3 );

}

#endif
