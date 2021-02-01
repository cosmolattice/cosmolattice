#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETJUMPS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETJUMPS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::GetJumps::Test(TempLat::TDDAssertion& tdd) {

    struct MyTestOne {
        JumpsHolder getJumps() {

            return JumpsHolder(LayoutStruct({4,4,4}), {{{{1, 1}}, {{1, 1}}, {{1, 1}}}});
        }
    };
    MyTestOne one;

    struct MyTestTwo {
    };
    MyTestTwo two;

    tdd.verify( GetJumps::apply(one).isEmpty() == false );
    tdd.verify( GetJumps::apply(two).isEmpty() == true );

}

#endif
