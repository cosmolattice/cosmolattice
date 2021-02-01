#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTSTATEKEEPER_TEST_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTSTATEKEEPER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::GhostStateKeeper::Test(TempLat::TDDAssertion& tdd) {

    GhostStateKeeper gsk;
    gsk.setUpToDate();
    tdd.verify( gsk.isUpToDate() && (gsk.isUpToDate() == ! gsk.isStale()) );
    gsk.setStale();
    tdd.verify( gsk.isStale() && (gsk.isUpToDate() == ! gsk.isStale()) );
    tdd.verify( gsk.needsUpdate() && (gsk.isUpToDate() == ! gsk.isStale()) );
    tdd.verify( gsk.isOutDated() && (gsk.isUpToDate() == ! gsk.isStale()) );

}

#endif
