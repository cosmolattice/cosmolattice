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
  auto fgSC = LatForwardGrad(sc);
  sc.updateGhosts(); // Need to force the ghost updating, as we do not assign the gradient to anything.

  bool OK = true;
  auto sc_view = sc.getLocalNDHostView();
  for (size_t i = 0; i < nGrid; ++i) {
    for (size_t j = 0; j < nGrid; ++j) {
      const double expect = (i + 1 < nGrid) ? 1.0 : (-nGrid + 1.0); // TODO
      if (std::abs(expect - sc_view(i, j)) > 1e-14) {
        OK = false;
        std::cout << "Mismatch at (" << i << ", " << j << "): "
                  << "expect = " << expect << ", SC = " << sc_view(i, j) << std::endl;
        break;
      }
    }
    if (!OK) break;
  }
  tdd.verify(OK);
}

#endif
