#ifndef TEMPLAT_UTIL_ASSIGNABLETUPLE_TEST_H
#define TEMPLAT_UTIL_ASSIGNABLETUPLE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::AssignableTupleTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */


    auto test = make_list(4, 5 ,6);

    auto change = make_list(1 ,2 , 3);

    test = change;

    tdd.verify( test(0_c)==1 );
    tdd.verify( test(1_c)==2 );
    tdd.verify( test(2_c)==3 );


    std::array<int,3> arr{3,4,5};

    auto res = make_list_from_array(arr);

    tdd.verify( res(0_c)==3 );
    tdd.verify( res(1_c)==4 );
    tdd.verify( res(2_c)==5 );
}

#endif
