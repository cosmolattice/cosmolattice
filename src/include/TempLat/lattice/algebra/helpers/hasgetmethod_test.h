#ifndef TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasGetMethodTester::Test(TempLat::TDDAssertion& tdd) {

    struct dummy {
        char a;
    };

    struct MyTestOne {
        dummy get(const ptrdiff_t& i) {
            std::cerr << "Hell yeah.\n";
            return dummy();
        }
    };

    struct MyTestTwo {
        void noGet() {
            std::cerr << "Hell no.\n";
        }
    };

    tdd.verify( HasGetMethod<MyTestOne>::value == true );
    tdd.verify( HasGetMethod<MyTestTwo>::value == false );
    tdd.verify( HasGetMethod<int>::value == false );

}

#endif
