#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYTOOLBOX_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYTOOLBOX_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::MemoryToolBox::Test(TempLat::TDDAssertion& tdd) {

    MemoryToolBox mtb(3, 256, 2);

// nothing to test: it is a collection of tools, tested elsewhere.
//    tdd.verify( true );

}

#endif
