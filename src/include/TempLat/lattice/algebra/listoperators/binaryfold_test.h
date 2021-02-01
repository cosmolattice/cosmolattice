#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_BINARYFOLD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_BINARYFOLD_TEST_H

 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::BinaryFoldTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    auto t1 = std::make_tuple(1,2,3,4);

    auto res = binary_fold([](auto x, auto y){return x+y;},t1, [](auto& x){return x;},0);


    tdd.verify( res == 10 );

    res = binary_fold([](auto x, auto y){return x*y;},t1, [](auto& x){return x;},1);
    tdd.verify( res == 24 );

    res = binary_fold([](auto x, auto y){return x+y;},t1, [](auto& x){return 2*x;},0);

    tdd.verify( res == 20 );



}

#endif
