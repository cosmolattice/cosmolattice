#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::IsComplexTypeTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( IsComplexType<std::complex<float>>::value );
    tdd.verify( IsComplexType<std::complex<double>>::value );
    tdd.verify( !IsComplexType<float>::value );
    tdd.verify( !IsComplexType<double>::value );

}

#endif
