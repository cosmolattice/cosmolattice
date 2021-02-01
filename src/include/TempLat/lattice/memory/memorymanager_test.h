#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYMANAGER_TEST_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYMANAGER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


template <typename T>
inline void TempLat::MemoryManager<T>::Test(TempLat::TDDAssertion& tdd) {
    
    auto toolBox = MemoryToolBox::makeShared(3, 192, 2);
    
    toolBox->mFFTLibrary.setVerbose();
    
    MemoryManager mManager( toolBox );

    if ( TDDRegister::isSingleUnitTest() ) {
        std::cerr << mManager << "\n\n";
        
        toolBox->setVerbose();
    }
    
    /* first allocation */
    tdd.verify(mManager.confirmConfigSpace() > 0);

    /* fft necessary */
    tdd.verify(mManager.confirmFourierSpace() > 0);

    /* fft not necessary */
    tdd.verify(mManager.confirmFourierSpace() == 0);

    /* fft necessary */
    tdd.verify(mManager.confirmConfigSpace() > 0);

    /* fft not necessary */
    tdd.verify(mManager.confirmConfigSpace() == 0);

    /* ghost update necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() > 0);

    /* ghost update not necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() == 0);

    /* fft not necessary */
    tdd.verify(mManager.confirmConfigSpace() == 0);

    /* ghost update not necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() == 0);

    /* fft necessary */
    tdd.verify(mManager.confirmFourierSpace() > 0);

    /* fft && ghost update necessary */
    tdd.verify(mManager.confirmGhostsUpToDate() > 0);

}

#endif
