#ifndef TEMPLAT_UTIL_ISTUPLELIKE_TEST_H
#define TEMPLAT_UTIL_ISTUPLELIKE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <array>

inline void TempLat::IsTupleLikeTester::Test(TempLat::TDDAssertion& tdd) {


    struct IAmNotATupleAintI{

    };

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( IsTupleLike< std::array<double, 4> >::value == true );
    tdd.verify( IsTupleLike< IAmNotATupleAintI >::value == false );
    tdd.verify( IsTupleLike< int >::value == false );

}

#endif
