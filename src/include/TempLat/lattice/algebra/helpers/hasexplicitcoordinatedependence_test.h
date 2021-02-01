#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEXPLICITCOORDINATEDEPENDENCE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEXPLICITCOORDINATEDEPENDENCE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

namespace TempLat {
    namespace TestScratch {
        struct DummyWithCoordinateDependence {
            static constexpr bool EXPLICITCOORDINATEDEPENDENCE = true;
        };
        struct DummyWithoutCoordinateDependence {
        };
        struct SecondDummyWithoutCoordinateDependence {
            static constexpr bool EXPLICITCOORDINATEDEPENDENCE = false;
        };
    }
}

inline void TempLat::HasExplicitCoordinateDependenceTester::Test(TempLat::TDDAssertion& tdd) {
    
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( HasExplicitCoordinateDependence<TestScratch::DummyWithCoordinateDependence>::value );
    
    tdd.verify( ! HasExplicitCoordinateDependence<TestScratch::DummyWithoutCoordinateDependence>::value );
    
    tdd.verify( ! HasExplicitCoordinateDependence<TestScratch::SecondDummyWithoutCoordinateDependence>::value );
    
}

#endif
