#ifndef TEMPLAT_LATTICE_FIELD_FIELD_TEST_H
#define TEMPLAT_LATTICE_FIELD_FIELD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

//#include "TempLat/lattice/algebra/gettergetoffset.h"
#include "TempLat/lattice/algebra/random/randomgaussianfield.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"


template<typename T>
inline void TempLat::Field<T>::Test(TempLat::TDDAssertion& tdd) {

    ptrdiff_t nGrid = 256, nGhost = 1;

    auto toolBox = MemoryToolBox::makeShared(3, nGrid, nGhost);

    toolBox->setVerbose();

    Field<T> phi("phi", toolBox);
    Field<T> chi("chi", toolBox);
    Field<T> psi("psi", toolBox);

    WaveNumber k(toolBox);

    phi.inFourierSpace() = k.norm2() * RandomGaussianField<T>("Hoi",toolBox);

    //just manipulated phi(k), so it must still be in Fourier space, and ghosts are stale.
    tdd.verify(phi.mManager->isFourierSpace());
    tdd.verify(phi.mManager->areGhostsStale());

    // alternatively, put the result of getNorm in a variable.
    //SpatialCoordinate x;
    //auto r = x.getNorm();

    chi = 3;//pow(r, 3);

    // just manipulated chi(x), so it must still be in configuration space, and ghosts are stale.
    tdd.verify(!chi.mManager->isFourierSpace());
    tdd.verify(chi.mManager->isConfigSpace());
    tdd.verify(chi.mManager->areGhostsStale());

    // neat consequence of the implementation: an expression actually evaluates to a specific type. Keeping that instance,
  //     without passing it to an assignment operator, is simply the compiled expression. So we can do stuff with it.
    auto potential = 0.5 * phi * phi + 42 * chi * chi * phi * phi - chi + (-chi);

    // Stuff we can do:
    say << "Potential2: " << potential.toString() << "\n";

    // and awesomer:
    auto dVdPhi = potential.d(phi);

    say << "dPotential/dphi: " << dVdPhi.toString() << "\n";

    // didn't touch ghost cells of phi, so ghosts must still be stale. Also, didn't touch phi at all, so must be in fourier space.
    tdd.verify(phi.mManager->isFourierSpace());
    tdd.verify(phi.mManager->areGhostsStale());

    psi = potential;

    // didn't touch ghost cells of phi, so ghosts must still be stale. Did read phi, so must be in configuration space.
    tdd.verify(phi.mManager->isConfigSpace());
    tdd.verify(phi.mManager->areGhostsStale());

    chi = phi + psi;

    chi.set(26,{3,4,5});

    bool bTest;

    auto tmp = chi(bTest, 3, 4, 5);

    if(bTest)
      tdd.verify(tmp == 26);

    // still didn't touch chi's ghost cells.
  /*  tdd.verify(chi.mManager->areGhostsStale());
    phi = chi.getShifted({{1, 1, -1}});
    // now did touch the ghost cells, must be up to date
    tdd.verify(!chi.mManager->areGhostsStale());

    phi = chi.getShifted({{1, 1, -1}});

    tdd.verify(phi.mManager->areGhostsStale());
// an early optimization attempt.
//    for (int i = 0; i < 1000; ++i ) {
        TimeSpent ts;
        psi = Laplacian(phi);
        std::cerr << ts << " spent.\n";
//    }
    tdd.verify(!phi.mManager->areGhostsStale());
*/



}

#endif
