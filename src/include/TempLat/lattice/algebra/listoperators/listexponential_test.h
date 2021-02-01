#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTEXPONENTIAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTEXPONENTIAL_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/foreach.h"
#include "TempLat/util/almostequal.h"

inline void TempLat::ListExponentialTester::Test(TempLat::TDDAssertion& tdd) {

    auto t1 = std::make_tuple(-1.,-2.,-3.,-4.);

    auto t3 = exp(t1) ;
    double tmp = -1;

    for_each(t3, [&](auto x){tdd.verify(AlmostEqual(x , std::exp(tmp--)));});

}

#endif
