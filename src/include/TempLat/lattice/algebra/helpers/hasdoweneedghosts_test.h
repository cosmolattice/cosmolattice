#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDOWENEEDGHOSTS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDOWENEEDGHOSTS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasDoWeNeedGhostsTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */
    struct MyTestOne {
        void doWeNeedGhosts() {
            std::cerr << "Hell yeah.\n";
        }
    };

    struct MyTestTwo {
        void notDoweNeedGHOST() {
            std::cerr << "Hell no.\n";
        }
    };

    struct MyTestThree {
        void doWeNeedGhosts(Tag<3> i) {
            std::cerr << "Hell yeah.\n";
        }
    };

    tdd.verify( HasDoWeNeedGhostsMethod<MyTestOne>::value == true );
    tdd.verify( HasDoWeNeedGhostsMethod<MyTestTwo>::value == false );
    tdd.verify( HasDoWeNeedGhostsMethod<int>::value == false );
    tdd.verify( HasDoWeNeedGhostsMethodIndexed<0,MyTestOne>::value == false );
    tdd.verify( HasDoWeNeedGhostsMethodIndexed<3,MyTestThree>::value == true );

}

#endif
