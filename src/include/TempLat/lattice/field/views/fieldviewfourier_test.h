#ifndef TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_TEST_H
#define TEMPLAT_LATTICE_FIELD_VIEWS_FIELDVIEWFOURIER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <atomic>

template<typename T>
inline void TempLat::FourierView<T>::Test(TempLat::TDDAssertion& tdd) {

    /*ptrdiff_t nGrid = 256, nGhost = 2;

    auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);

    toolBox->setVerbose();

    FieldChainFinal<T> phiBase("phi", toolBox);
    FieldViewFourier<T> phi(phiBase);*/

    /* test that the looper is what we expected */

    /*std::atomic<bool> lastDimPositiveDefinite(true);

    WaveNumber k;

    phi.iterate( [&](IterationCoordinates &pIterCoords) {
        bool localSuccess = pIterCoords[2] >= 0;*/
        /* atomically put the new value in place, but if we had success, it is important
          that we don't overwrite another thread's failure. */
        /*bool expected = true;
        lastDimPositiveDefinite.compare_exchange_weak(expected, localSuccess);
//        say << offset << " localSuccess: " << localSuccess << " pIterCoords[2]: " << pIterCoords[2] << "\n";
        return localSuccess;
    });

    tdd.verify(lastDimPositiveDefinite == true);*/



}

#endif
