#ifndef TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALSTREAM_TEST_H
#define TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALSTREAM_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::ConditionalStream::Test(TempLat::TDDAssertion& tdd) {
    
    std::stringstream str1, str2;
    ConditionalStream one(str1, true);
    ConditionalStream two(str2, false);
    
    one << "Hoi!";
    two << "Doei!";
    
    tdd.verify(str1.str().length() > 0);
    tdd.verify(str2.str().length() == 0);
 
}

#endif
