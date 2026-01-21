#ifndef TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_TEST_H
#define TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"
#include <iomanip> // setprecision
#include <vector>

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

    // Test saveState/loadState round-trip
    RandomUniform<> rng("serialization_test");
    std::string savedState = rng.saveState();

    // Generate 1000 values after saving state
    std::vector<double> seq1;
    for (int i = 0; i < 1000; ++i) {
        seq1.push_back(rng());
    }

    // Restore state and generate again
    rng.loadState(savedState);
    std::vector<double> seq2;
    for (int i = 0; i < 1000; ++i) {
        seq2.push_back(rng());
    }

    // Verify sequences are identical
    bool sequencesMatch = true;
    for (int i = 0; i < 1000; ++i) {
        if (seq1[i] != seq2[i]) {
            sequencesMatch = false;
            break;
        }
    }
    tdd.verify(sequencesMatch, "saveState/loadState round-trip produces identical sequence");

}

#endif
