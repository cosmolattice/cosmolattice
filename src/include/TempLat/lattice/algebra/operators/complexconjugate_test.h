#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_TEST_H
#include <Kokkos_Macros.hpp>

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::ComplexConjugateTester::Test(TempLat::TDDAssertion& tdd) {

    struct myStruct {
        KOKKOS_FORCEINLINE_FUNCTION
        complex<double> get(ptrdiff_t i) {
            return complex<double>(1, -1);
        }
    };

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( myStruct().get(0) == complex<double>(1, -1) );

    tdd.verify( conj(myStruct()).get(0) == complex<double>(1, 1) );

    /* test: this should not compile. Passed. */
//    struct myFailStruct {
//        double get(IterationCoordinates&) {
//            return 1;
//        }
//    };
//
//    tdd.verify( conj(myFailStruct()).get(pIterCoords) == complex<double>(1, 1) );
}

#endif
