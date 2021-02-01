#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasVectorGetMethodTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    struct dummy {
        char a;
    };

    struct MyTestOne {
        dummy vectorGet(const ptrdiff_t& i,const ptrdiff_t& j) {
            std::cerr << "Hell yeah.\n";
            return dummy();
        }
    };

    struct MyTestTwo {
        void noGet() {
            std::cerr << "Hell no.\n";
        }
    };

    tdd.verify( HasVectorGetMethod<MyTestOne>::value == true );
    tdd.verify( HasVectorGetMethod<MyTestTwo>::value == false );
    tdd.verify( HasVectorGetMethod<int>::value == false );


}

#endif
