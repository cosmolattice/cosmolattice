#ifndef TEMPLAT_UTIL_TUPLE_SIZE_TESTER_TEST_H
#define TEMPLAT_UTIL_TUPLE_SIZE_TESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tuple_size.h"

struct testtuple{
    static const size_t size = 42;
};

inline void TempLat::tuple_size_Tester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( tuple_size<testtuple>::value == 42 );
    tdd.verify( tuple_size<int>::value == 1 );
    tdd.verify( tuple_size<std::tuple<int,int,int>>::value == 3 );

}

#endif
