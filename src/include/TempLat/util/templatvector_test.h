#ifndef TEMPLAT_UTIL_TempLatVECTOR_TEST_H
#define TEMPLAT_UTIL_TempLatVECTOR_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

template<typename T, int N>
inline void TempLat::TempLatVector<T, N>::Test(TempLat::TDDAssertion& tdd) {

    TempLatVector<int> vec(1,2,3);

    tdd.verify( vec.getComp(1_c) == 2 );

    std::vector<int> sVec(3,6);

    vec = sVec;

    tdd.verify( vec[2] == 6 );


}

#endif
