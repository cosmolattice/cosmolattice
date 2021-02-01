#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTCOMPLEXCONJUGATE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTCOMPLEXCONJUGATE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::ListComplexConjugateTester::Test(TempLat::TDDAssertion& tdd) {

    std::complex<double> tmp(1,1);
    auto t1 = std::make_tuple(tmp,tmp,tmp,tmp);

    auto t3 = conj(t1);
    tdd.verify( GetComponent::get(t3,Tag<0>()) == std::conj(tmp) );
}

#endif
