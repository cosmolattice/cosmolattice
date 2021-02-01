#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALBINCOMPUTER_TEST_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALBINCOMPUTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::RadialBinComputer::Test(TempLat::TDDAssertion& tdd) {

    RadialBinComputer pc(1, 3, 4);
    
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( pc(1.1) == 0 );
    tdd.verify( pc(0) == 0 );

    tdd.verify( pc(2.1) == 2 );
    tdd.verify( pc(5) == 3 );

}

#endif
