#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETCOMPONENT_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETCOMPONENT_TEST_H


 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019


inline void TempLat::GetComponentTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    auto v1 = std::make_tuple(87.0, 2);


    tdd.verify( GetComponent::get(v1, Tag<1>()) == 2 );
    tdd.verify( GetComponent::get(35, Tag<1>()) == 35 );

}

#endif
