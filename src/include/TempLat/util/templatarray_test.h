#ifndef TEMPLAT_UTIL_TempLatARRAY_TEST_H
#define TEMPLAT_UTIL_TempLatARRAY_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

template <class T, int N, int M>
inline void TempLat::TempLatArray<T,N, M>::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( true );

}

#endif
