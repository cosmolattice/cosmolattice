#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_FORWARDGRADIENTLOCAL_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"

inline void TempLat::LatticeForwardGradientTester::Test(TempLat::TDDAssertion &tdd)
{
  constexpr size_t nd = 2;
  const ptrdiff_t nGrid = 32, nGhost = 1;

  auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);
  SpatialCoordinate x(toolBox);
  toolBox->setVerbose();

  Field<nd, double> sc("SC1", toolBox);
  sc = getVectorComponent(x, 0);
  auto fgSC = LatForwardGrad(sc).norm2();
  sc.updateGhosts(); // Need to force the ghost updating, as we do not assign the gradient to anything.

  bool OK = true;
  auto it = toolBox->itX();
  for (it.begin(); it.end(); ++it) {
    auto coord = toolBox->getCoordConfiguration(it());
    if (coord[0] != 16) // coordinates range from -15 to 16.
      OK = OK && fgSC.get(it()) == 1;
    else
      OK = OK && fgSC.get(it()) == 961; // Go accross the boundary need to get (-15-16)^2 == 961
  }
  tdd.verify(OK);

  auto fgSC2 = LatForwardGrad(3 * sc).norm2();
  say << fgSC2.getJumps();
  OK = true;
  for (it.begin(); it.end(); ++it) {
    auto coord = toolBox->getCoordConfiguration(it());
    if (coord[0] != 16) // coordinates range from -15 to 16.
      OK = OK && fgSC2.get(it()) == 3 * 3;
    else
      OK = OK && fgSC2.get(it()) == 961 * 9; // Go accross the boundary need to get (-15-16)^2 == 961
  }
  tdd.verify(OK);

  auto fgSC3 = LatForwardGrad(sc + sc).norm2();
  OK = true;
  for (it.begin(); it.end(); ++it) {
    auto coord = toolBox->getCoordConfiguration(it());
    if (coord[0] != 16) // coordinates range from -15 to 16.
      OK = OK && fgSC3.get(it()) == 2 * 2;
    else
      OK = OK && fgSC3.get(it()) == 961 * 4; // Go accross the boundary need to get (-15-16)^2 == 961
  }
  tdd.verify(OK);
}

#endif
