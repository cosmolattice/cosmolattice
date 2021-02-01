#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::HasSpaceConfirmationMethodsTester::Test(TempLat::TDDAssertion& tdd) {

    struct MyTestOne {
        void confirmSpace(const LayoutStruct& newLayout, const SpaceStateInterface::SpaceType& sType) {
            std::cerr << "Hell yeah.\n";
        }
    };

    struct MyTestTwo {
        void notConfirmConfigSpace() {
            std::cerr << "Hell no.\n";
        }
    };

    struct MyTestThree {
        void confirmSpace(Tag<3> i, const LayoutStruct& newLayout, const SpaceStateInterface::SpaceType& sType) {
            std::cerr << "Hell yeah.\n";
        }
    };

    tdd.verify( HasSpaceConfirmationMethods<MyTestOne>::value == true );
    tdd.verify( HasSpaceConfirmationMethods<MyTestTwo>::value == false );
    tdd.verify( HasSpaceConfirmationMethods<int>::value == false );
    tdd.verify( HasSpaceConfirmationMethodsIndexed<3,MyTestOne>::value == false );
    tdd.verify( HasSpaceConfirmationMethodsIndexed<3,MyTestThree>::value == true );

}

#endif
