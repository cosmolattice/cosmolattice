#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTSTATE_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTSTATE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

inline void TempLat::MemoryLayoutState::Test(TempLat::TDDAssertion& tdd) {

    MemoryLayoutState mState;
    
    tdd.verify( mState.isConfigSpace() && mState.isFFTConfigSpace() && mState.isFourierSpace() );
    
    mState.setToConfigSpace();

    tdd.verify( mState.isConfigSpace() && !mState.isFFTConfigSpace() && !mState.isFourierSpace() );

    mState.setToFFTConfigSpace();

    tdd.verify( !mState.isConfigSpace() && mState.isFFTConfigSpace() && !mState.isFourierSpace() );

    mState.setToFourierSpace();

    tdd.verify( !mState.isConfigSpace() && !mState.isFFTConfigSpace() && mState.isFourierSpace() );

}

#endif
