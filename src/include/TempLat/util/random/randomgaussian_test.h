#ifndef TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_TEST_H
#define TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>
#include <vector>

inline void TempLat::Util::RandomGaussian::Test(TempLat::TDDAssertion& tdd) {

    RandomGaussian prng("Hello CosmoLattice world!");
    
//    say << prng << "\n";
    
    static const ptrdiff_t measure_center = 10;
    std::array<size_t, 2 * measure_center> measure;
    std::fill(measure.begin(), measure.end(), 0);
    double x = 0;
    for ( int i = 0; i < 10000; ++i ) {
        double next = prng();
        x += next;
        ptrdiff_t index = measure_center + std::round(next * measure_center / 3); /* 5 ? yes, 5 i_sigma happens. */
        index = std::max(ptrdiff_t(0), std::min(2 * measure_center - 1, index));
        ++measure[index];
    }
//    std::cerr << std::fixed << std::setprecision(32) << "x: " << x << "\n";
    
    tdd.verify( prng.getState() == 10000u );
    tdd.verify( AlmostEqual(x, 63.06717395285973282170743914321065) );
//    say << prng << "\n";

    say << "Does this look gaussian enough?\n";
    for ( auto&& it : measure ) {
        if ( it > 0 ) std::cerr << std::string(it / 100, '*') << "\n";
    }

    // Test saveState/loadState round-trip
    RandomGaussian rng("serialization_test");
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

    // Also test with cached value (odd number of draws)
    RandomGaussian rng2("cache_test");
    rng2();  // Draw one value to create cached state
    std::string savedStateWithCache = rng2.saveState();

    std::vector<double> seq3;
    for (int i = 0; i < 999; ++i) {
        seq3.push_back(rng2());
    }

    rng2.loadState(savedStateWithCache);
    std::vector<double> seq4;
    for (int i = 0; i < 999; ++i) {
        seq4.push_back(rng2());
    }

    bool cacheSequencesMatch = true;
    for (int i = 0; i < 999; ++i) {
        if (seq3[i] != seq4[i]) {
            cacheSequencesMatch = false;
            break;
        }
    }
    tdd.verify(cacheSequencesMatch, "saveState/loadState with cached value produces identical sequence");

}

#endif
