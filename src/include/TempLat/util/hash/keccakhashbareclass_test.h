#ifndef TEMPLAT_UTIL_HASH_KECCAKHASHBARECLASS_TEST_H
#define TEMPLAT_UTIL_HASH_KECCAKHASHBARECLASS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::KeccakHashBareClass::Test(TempLat::TDDAssertion& tdd) {

    KeccakHashBareClass khbc;
    khbc.compute("Anything.");
    /* check that we cannot compute twice on the same object. */
    tdd.verify( Throws<KeccakHashBareClassReuseException>( [&]() { khbc.compute("Anything."); })  );

}

#endif
