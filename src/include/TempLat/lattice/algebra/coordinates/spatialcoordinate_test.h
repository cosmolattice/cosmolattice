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
  ptrdiff_t nGrid = 16, nGhost = 2;

  auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

  Field<NDim, double> phix("phix", toolBox);
  Field<NDim, double> phiy("phiy", toolBox);

  SpatialCoordinate<NDim> x(toolBox);
  phix = getVectorComponent(x, 0);
  phiy = getVectorComponent(x, 1);

  auto phix_view = phix.getLocalNDHostView();
  auto phiy_view = phiy.getLocalNDHostView();

  auto layout = toolBox->mLayouts.getConfigSpaceLayout();

  // Check that the spatial coordinate is correct
  bool correct = true;
  for (ptrdiff_t i = 0; i < phix_view.extent(0); ++i) {
    for (ptrdiff_t j = 0; j < phix_view.extent(1); ++j) {
      const ptrdiff_t global_x = layout.getLocalStarts()[0] + i;
      const ptrdiff_t global_y = layout.getLocalStarts()[1] + j;

      const ptrdiff_t x_val = global_x > nGrid / 2 ? global_x - nGrid : global_x;
      const ptrdiff_t y_val = global_y > nGrid / 2 ? global_y - nGrid : global_y;

      correct &= (phix_view(i, j) == x_val) && (x.vectorGet(0, nGhost + i, nGhost + j) == x_val);
      correct &= (phiy_view(i, j) == y_val) && (x.vectorGet(1, nGhost + i, nGhost + j) == y_val);

      if (!(phix_view(i, j) == x_val))
        sayMPI << "Failed: phix.get(" << i << ", " << j << ") = " << phix_view(i, j) << ", expect " << x_val << "\n";
      if (!(phiy_view(i, j) == y_val))
        sayMPI << "Failed: phiy.get(" << i << ", " << j << ") = " << phiy_view(i, j) << ", expect " << y_val << "\n";
    }
  }
  tdd.verify(correct);
}

#endif
