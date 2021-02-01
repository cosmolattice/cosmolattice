#ifndef TEMPLAT_UTIL_ISINCONTAINER_TEST_H
#define TEMPLAT_UTIL_ISINCONTAINER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::IsInContainer::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( check(1,std::vector<int>{1,2,3,4}) );
    tdd.verify(! check(1,std::vector<int>{2,3,4}) );
    tdd.verify( check("E_K",std::vector<std::string>{"a","E_K","b"}) );

}

#endif
