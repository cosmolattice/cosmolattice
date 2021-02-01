#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasJumpMethodsTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyTestOne {
        JumpsHolder getJumps() {
            std::cerr << "Hell yeah.\n";
            return JumpsHolder();
        }
    };

    struct MyTestTwo {
        JumpsHolder getNOConfigSpaceJumps() {
            std::cerr << "Hell no.\n";
            return JumpsHolder();
        }
    };

    tdd.verify( HasJumpMethods<MyTestOne>::value == true );
    tdd.verify( HasJumpMethods<MyTestTwo>::value == false );
    tdd.verify( HasJumpMethods<int>::value == false );

}

#endif
