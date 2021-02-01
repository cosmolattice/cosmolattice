#ifndef TEMPLAT_UTIL_LOG_SAYCOMPLETE_TEST_H
#define TEMPLAT_UTIL_LOG_SAYCOMPLETE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


template <typename TestObjectUnknownHere>
inline void TempLat::SayCompleteTest::Test(TestObjectUnknownHere& tdd) {
    
    say << "Stream log - first light.\n";
    sayShort << "Shorter stream log - first light.\n";
    tdd.verify( true );

}

#endif
