#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETSTRING_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETSTRING_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::GetString::Test(TempLat::TDDAssertion& tdd) {

    tdd.verify( std::is_same<std::string, decltype(GetString::get(std::complex<double>(1, 1)))>::value );

    tdd.verify( std::is_same<std::string, decltype(GetString::get((double)1))>::value );

    //tdd.verify( std::is_same<std::string, decltype(GetString::get(std::complex<double>(1, 1),2))>::value );
}

#endif
