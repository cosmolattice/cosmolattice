#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONREBINNER_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONREBINNER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

template <typename T>
inline void TempLat::RadialProjectionRebinner<T>::Test(TempLat::TDDAssertion& tdd) {

    say << "See fcn/lattice/measuringtoolsIO/radialprojection_test.h\n";
    tdd.verify( true );

}

#endif
