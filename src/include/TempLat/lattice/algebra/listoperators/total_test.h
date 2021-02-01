#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_TOTAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_TOTAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::TotalTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    auto test = std::make_tuple(1,2,3,4);

    tdd.verify( total(test) == 10 );
    tdd.verify( total(test, [](auto x){return 2*x;}) == 20 );

}

#endif
