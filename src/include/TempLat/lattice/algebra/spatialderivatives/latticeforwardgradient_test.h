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
  const ptrdiff_t nGrid = 8, nGhost = 1;

  auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);
  SpatialCoordinate x(toolBox);
  toolBox->setVerbose();

  Field<nd, double> sc("SC1", toolBox);
  sc = getVectorComponent(x, 0);
  Field<nd, double> fgsc("fgSC1", toolBox);
  fgsc = getVectorComponent(LatForwardGrad<nd>(sc), 0);

  bool OK = true;
  auto sc_view = sc.getLocalNDHostView();
  auto fgsc_view = fgsc.getLocalNDHostView();
  for (size_t i = 0; i < nGrid; ++i) {
    for (size_t j = 0; j < nGrid; ++j) {
      const double expect = sc_view(i + 1, j) - sc_view(i, j);
      if (std::abs(expect - fgsc_view(i, j)) > 1e-14) {
        OK = false;
        std::cout << "Mismatch at (" << i << ", " << j << "): "
                  << "expect = " << expect << ", fgSC = " << fgsc_view(i, j) << std::endl;
      }
    }
    if (!OK) break;
  }
  tdd.verify(OK);
}

#endif
