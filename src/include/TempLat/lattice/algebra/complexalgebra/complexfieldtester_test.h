#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDTESTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDTESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/algebra/complexalgebra/complexfield.h"
#include "TempLat/lattice/algebra/complexalgebra/complexalgebra.h"


inline void TempLat::ComplexFieldTester::Test(TempLat::TDDAssertion& tdd) {

    int nDim = 3;
    int nGrid = 32;
    int nGhost = 0;

    auto toolBox = MemoryToolBox::makeShared(nDim, nGrid, nGhost);
    WaveNumber k(toolBox);

    ComplexField<double> phi("phi",toolBox);
    ComplexField<double> xi("xi",toolBox);

    tdd.verify(phi.inFourierSpace()(0_c).isFourierSpace());
    tdd.verify(phi.inFourierSpace()(1_c).isFourierSpace());

    auto test = k[1] * phi.inFourierSpace();

    say << test;

    xi.inFourierSpace() = k[1] * phi.inFourierSpace();
    xi.inFourierSpace() = xi.inFourierSpace() * phi.inFourierSpace();

    tdd.verify(xi.inFourierSpace()(0_c).isFourierSpace());
    tdd.verify(xi.inFourierSpace()(1_c).isFourierSpace());

    xi = phi;
    tdd.verify(xi(0_c).isConfigSpace());
    tdd.verify(xi(1_c).isConfigSpace());






}

#endif
