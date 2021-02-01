#ifndef TEMPLAT_UTIL_GETCPPTYPENAME_TEST_H
#define TEMPLAT_UTIL_GETCPPTYPENAME_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::GetCPPTypeName::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( GetCPPTypeName::get(std::vector<ptrdiff_t>()).find("std::") != std::string::npos );
    tdd.verify( GetCPPTypeName::get(std::vector<ptrdiff_t>()).find("::vector") != std::string::npos );

}

#endif
