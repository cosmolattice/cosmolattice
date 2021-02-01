#ifndef TEMPLAT_UTIL_ENDIANNESS_TEST_H
#define TEMPLAT_UTIL_ENDIANNESS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::Endianness::Test(TempLat::TDDAssertion& tdd) {

    Endianness endian;
    tdd.verify( endian.isLittle() );
    tdd.verify( ! endian.isBig() );

}

#endif
