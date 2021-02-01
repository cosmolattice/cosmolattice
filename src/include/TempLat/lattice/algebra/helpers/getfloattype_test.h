#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETFLOATTYPE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETFLOATTYPE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::GetFloatTypeTester::Test(TempLat::TDDAssertion& tdd) {


    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( std::is_same<float, GetFloatType<float>::type>::value );

    tdd.verify( std::is_same<float, GetFloatType<std::complex<float>>::type>::value );

    tdd.verify( std::is_same<double, GetFloatType<double>::type>::value );

    tdd.verify( std::is_same<double, GetFloatType<std::complex<double>>::type>::value );

    tdd.verify( !std::is_same<double, GetFloatType<float>::type>::value );

    tdd.verify( !std::is_same<float, GetFloatType<std::complex<double>>::type>::value );


}

#endif
