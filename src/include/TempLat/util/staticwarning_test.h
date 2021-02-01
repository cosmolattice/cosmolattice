#ifndef TEMPLAT_UTIL_STATICWARNING_TEST_H
#define TEMPLAT_UTIL_STATICWARNING_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::StaticWarning::Test(TempLat::TDDAssertion& tdd) {

// uncomment if you are actually testing this thing. Comment otherwise, because you would see a warning every time this header is included somewhere.
//    static_warn(false, "You must see this at compilation.");

    static_warn(true, "You must NEVER see this at compilation.");

}

#endif
