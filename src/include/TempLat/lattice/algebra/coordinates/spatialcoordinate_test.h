#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber.h"

#include <iostream>

template <size_t NDim_> inline void TempLat::SpatialCoordinate<NDim_>::Test(TempLat::TDDAssertion &tdd)
{
  static constexpr size_t NDim = 2;
  ptrdiff_t nGrid = 8, nGhost = 0;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

  Field<NDim, double> phix("phix", toolBox);
  Field<NDim, double> phiy("phiy", toolBox);

  SpatialCoordinate<NDim> x(toolBox);
  phix = getVectorComponent(x, 0);
  phiy = getVectorComponent(x, 1);

  auto phix_view = phix.getLocalNDHostView();
  auto phiy_view = phiy.getLocalNDHostView();

  // Check that the spatial coordinate is correct
  bool correct = true;
  for (ptrdiff_t i = 0; i < nGrid; ++i) {
    for (ptrdiff_t j = 0; j < nGrid; ++j) {
      const ptrdiff_t x_val = i > nGrid / 2 ? i - nGrid : i;
      correct &= phix_view(i, j) == x_val;
      const ptrdiff_t y_val = j > nGrid / 2 ? j - nGrid : j;
      correct &= phiy_view(i, j) == y_val;

      std::cout << "phix.get(" << i << ", " << j << ") = " << phix_view(i, j) << ", expect " << x_val << "\n";
      std::cout << "phiy.get(" << i << ", " << j << ") = " << phiy_view(i, j) << ", expect " << y_val << "\n";
    }
  }
  tdd.verify(correct);
}

#endif
