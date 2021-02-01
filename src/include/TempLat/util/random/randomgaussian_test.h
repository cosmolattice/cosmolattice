#ifndef TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_TEST_H
#define TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>

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

}

#endif
