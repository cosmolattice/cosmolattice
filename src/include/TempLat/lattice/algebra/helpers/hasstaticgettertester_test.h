#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTERTESTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTERTESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/lattice/field/field.h"


inline void TempLat::HasStaticGetterTester::Test(TempLat::TDDAssertion& tdd) {

    auto test = Complexify(1 ,2);

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( HasStaticGetter<decltype(test)>::value == true );
    tdd.verify( HasStaticGetter<double>::value == false );
    tdd.verify( HasStaticGetter<Field<double>>::value == false );

}

#endif
