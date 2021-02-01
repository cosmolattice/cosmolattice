#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTPOWER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTPOWER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/foreach.h"
#include "TempLat/util/almostequal.h"
inline void TempLat::ListPowerTester::Test(TempLat::TDDAssertion& tdd) {

    auto t1 = std::make_tuple(1.0,2.0,3.0,4.0);
    auto t2 = std::make_tuple(5.0,6.0,7.0,8.0);

    auto t3 = pow(t1 , t2);

    double tmp1 = 1.0;
    double tmp2 = 5.0;

    for_each(t3, [&](auto x){tdd.verify(AlmostEqual(x, std::pow(tmp1++,tmp2++)));});
    auto t4 = pow(t1 , 9.0);
    auto t5 = pow<9>(t1 );
    say << GetComponent::get(t4,Tag<1>());
    tdd.verify(  AlmostEqual(GetComponent::get(t4,Tag<1>()) ,std::pow(2.0,9.0)  ));
    tdd.verify(  AlmostEqual(GetComponent::get(t5,Tag<2>()) ,std::pow(3.0,9.0)  ));

}

#endif
