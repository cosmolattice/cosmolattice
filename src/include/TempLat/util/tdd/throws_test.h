#ifndef TEMPLAT_UTIL_TDD_THROWS_TEST_H
#define TEMPLAT_UTIL_TDD_THROWS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


template <typename TestObjectUnknownHere>
inline void TempLat::ThrowsTester::Test(TestObjectUnknownHere& tdd) {

    tdd.verify( Throws<std::runtime_error> (
                                            []() {
                                                throw std::runtime_error("Test throw: should work.");
                                            }
                                            )
               );

}

#endif
