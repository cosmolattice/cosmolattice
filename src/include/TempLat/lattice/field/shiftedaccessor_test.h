#ifndef TEMPLAT_LATTICE_FIELD_SHIFTEDACCESSOR_H_SHIFTEDACCESSOR_TEST_H
#define TEMPLAT_LATTICE_FIELD_SHIFTEDACCESSOR_H_SHIFTEDACCESSOR_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/memory/memorytoolbox.h"

inline void TempLat::ShiftedAccessor::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    ptrdiff_t nGrid = 256, nGhost = 1;
    auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);

    toolBox->setVerbose();
    //Field<double> phi("phi", toolBox);
    //WaveNumber k;
    //phi.inFourierSpace() = k.getNormSquared() * RandomGaussianField<double>("Hoi");
    //phi = pow(phi, 3);

    //auto itX=toolBox->itX();
    //itX.begin();
    //++itX;

    ShiftedAccessor shift(toolBox->mLayouts.getConfigSpaceJumps(),{0,1,0});


    //say<<phi(itX);
    //say<<shift(phi,itX);


}

#endif
