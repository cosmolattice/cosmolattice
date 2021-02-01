#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTMULTIPLY_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTMULTIPLY_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/foreach.h"
#include "TempLat/util/almostequal.h"
inline void TempLat::ListMultiplicationTester::Test(TempLat::TDDAssertion& tdd) {


    auto t1 = std::make_tuple(1.0,2.0,3.0,4.0);
    auto t2 = std::make_tuple(5.0,6.0,7.0,8.0);

    auto t3 = t1 * t2;

    double tmp1 = 1;
    double tmp2 = 5.0;

    for_each(t3, [&](auto x){tdd.verify(AlmostEqual(x, (tmp1++)*(tmp2++)));});
    auto t4 = t1 * 9.0;
    tdd.verify(  AlmostEqual(GetComponent::get(t4,Tag<0>()) ,1.0*9.0)  );


}

#endif
