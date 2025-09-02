#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETAVERAGER_TEST_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETAVERAGER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/su2algebra/su2doublet.h"

inline void TempLat::SU2DoubletAveragerTester::Test(TempLat::TDDAssertion &tdd)
{
  auto toolbox = MemoryToolBox<3>::makeShared(16, 1);

  SU2Doublet<3, double> A("A", toolbox);

  A(0_c) = 0.1;
  A(1_c) = 0.2;
  A(2_c) = 0.3;
  A(3_c) = 0.4;

  auto average = su2doubletaverage(A);

  tdd.verify(AlmostEqual(average.getComp(0_c), 0.1));
  tdd.verify(AlmostEqual(average.getComp(1_c), 0.2));
  tdd.verify(AlmostEqual(average.getComp(2_c), 0.3));
  tdd.verify(AlmostEqual(average.getComp(3_c), 0.4));
}

#endif
