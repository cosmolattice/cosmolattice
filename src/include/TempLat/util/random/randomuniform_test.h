#ifndef TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_TEST_H
#define TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"
#include <iomanip> // setprecision

inline void TempLat::RandomUniformTester::Test(TempLat::TDDAssertion& tdd) {

    RandomUniform<> prng("Hello CosmoLattice world!");
    
//    say << prng << "\n";
    
    double x = 0;
    for ( int i = 0; i < 1000; ++i ) {
        x += prng();
    }
//    std::cerr << std::fixed << std::setprecision(32) << "x: " << x << "\n";
    
    tdd.verify( prng.getState() == 1000u );
    tdd.verify( AlmostEqual(x, 489.76572307423589336394798010587692) );

}

#endif
