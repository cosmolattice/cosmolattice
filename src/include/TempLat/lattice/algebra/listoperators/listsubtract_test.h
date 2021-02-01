#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_VECTORSUBTRACT_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_VECTORSUBTRACT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/foreach.h"



inline void TempLat::ListSubtractTester::Test(TempLat::TDDAssertion& tdd) {

    auto t1 = std::make_tuple(1,2,3,4);
    auto t2 = std::make_tuple(5,6,7,8);

    auto t3 = t1 - t2;

    for_each(t3, [&](auto x){tdd.verify(x ==-4);});
    auto t4 =t1 - 9;
    tdd.verify( GetComponent::get(t4,Tag<0>()) == -8  );

}

#endif
