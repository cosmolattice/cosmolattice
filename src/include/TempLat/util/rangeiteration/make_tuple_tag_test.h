#ifndef TEMPLAT_UTIL_RANGEITERATION_MAKE_TUPLE_TAG_TEST_H
#define TEMPLAT_UTIL_RANGEITERATION_MAKE_TUPLE_TAG_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/foreach.h"

inline void TempLat::make_tuple_tagTester::Test(TempLat::TDDAssertion& tdd) {

    auto test = make_tuple_tag<10>([](auto i){return 2 * i;});

    int i =0;
    bool tmp = true;

    for_each(test,[&](auto x){tmp = tmp && x == 2*i++ ;});

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( tmp );

}

#endif
