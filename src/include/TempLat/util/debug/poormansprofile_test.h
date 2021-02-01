#ifndef TEMPLAT_UTIL_DEBUG_POORMANSPROFILE_TEST_H
#define TEMPLAT_UTIL_DEBUG_POORMANSPROFILE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::PoorMansProfile::Test(TempLat::TDDAssertion& tdd) {

    if ( TDDRegister::isSingleUnitTest() ) {
        
        auto&& f1 = []() {
            PoorMansProfile::TrackMe();
        };
        auto&& f2 = []() {
            PoorMansProfile::TrackMe();
        };
        for ( int i = 0; i < 100; ++i) {
            f1();
            if ( i % 3 == 0 ) f2();
        }
    }
}

#endif
