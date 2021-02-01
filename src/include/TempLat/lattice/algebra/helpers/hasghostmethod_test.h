#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasGhostMethodTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyTestOne {
        ptrdiff_t confirmGhostsUpToDate() {
            std::cerr << "Hell yeah.\n";
            return 0;
        }
    };

    struct MyTestTwo {
        ptrdiff_t notConfirmGhostsUpToDate() {
            std::cerr << "Hell no.\n";
            return 0;
        }
    };

    struct MyTestThree {
        ptrdiff_t confirmGhostsUpToDate(Tag<3> i) {
            std::cerr << "Hell yeah.\n";
            return 0;
        }
    };

    tdd.verify( HasGhostMethod<MyTestOne>::value == true );
    tdd.verify( HasGhostMethod<MyTestTwo>::value == false );
    tdd.verify( HasGhostMethod<int>::value == false );
    tdd.verify( HasGhostMethodIndexed<3,MyTestOne>::value == false );
    tdd.verify( HasGhostMethodIndexed<3,MyTestThree>::value == true );

}

#endif
