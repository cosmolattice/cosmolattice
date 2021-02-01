#ifndef TEMPLAT_LATTICE_ALGEBRA_HASSTRINGMETHOD_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HASSTRINGMETHOD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasStringMethodTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyTestOne {
        void toString() {
            std::cerr << "Hell yeah.\n";
        }
    };

    struct MyTestTwo {
        void notToString() {
            std::cerr << "Hell no.\n";
        }
    };

    struct MyTestThree {
        void toString(Tag<3> i) {
            std::cerr << "Hell yes.\n";
        }
    };

    tdd.verify( HasStringMethod<MyTestOne>::value == true );
    tdd.verify( HasStringMethod<MyTestTwo>::value == false );
    tdd.verify( HasStringMethod<int>::value == false );
    tdd.verify( HasStringMethodIndexed<3,MyTestOne>::value == false );
    tdd.verify( HasStringMethodIndexed<3,MyTestThree>::value == true );

}

#endif
