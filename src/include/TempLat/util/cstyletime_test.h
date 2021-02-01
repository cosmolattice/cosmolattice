#ifndef TEMPLAT_UTIL_CSTYLETIME_TEST_H
#define TEMPLAT_UTIL_CSTYLETIME_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::CStyleTime::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    CStyleTime mtime;
    mtime.now();
    say << mtime.date();
    say << mtime.time();
    tdd.verify( true );

}

#endif
