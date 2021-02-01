#ifndef TEMPLAT_UTIL_LOADBALANCE_TEST_H
#define TEMPLAT_UTIL_LOADBALANCE_TEST_H
#include <cstddef>

 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::LoadBalance::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    auto sum_this_vec = [](auto x){ptrdiff_t res = 0; for(auto t : x) res +=t; return res;};

    auto tes = LoadBalance::getConf(64, 3 );
    tdd.verify(tes[0] == 22 && tes[1] == 21 && tes[2] == 21);
    tes = LoadBalance::getConf(64, 7 );
    tdd.verify(sum_this_vec(tes) == 64);
    tes = LoadBalance::getConf(637646557, 6543 );
    tdd.verify(sum_this_vec(tes) == 637646557);

}

#endif
