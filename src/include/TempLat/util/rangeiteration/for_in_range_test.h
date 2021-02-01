#ifndef TEMPLAT_UTIL_RANGEITERATION_FOR_IN_RANGE_TEST_H
#define TEMPLAT_UTIL_RANGEITERATION_FOR_IN_RANGE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::for_in_range_Tester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    int c = 1;
    for_in_range<1,4>(
      [&](auto i)
      {
        tdd.verify( i == c++ );
      }
    );
    ForLoop(i, 0, 0, say << (int) i);

}

#endif
