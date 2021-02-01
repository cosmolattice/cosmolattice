#ifndef TEMPLAT_UTIL_PRETTYTOSTRING_TEST_H
#define TEMPLAT_UTIL_PRETTYTOSTRING_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::PrettyToString::Test(TempLat::TDDAssertion& tdd) {


    say << get(1.0);
    say << get(1.000001);
    say << get(1.000002);
    say << get(1.000002003);
    tdd.verify(true);


}

#endif
