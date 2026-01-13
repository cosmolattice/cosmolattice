#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_VECTORDOTTER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/listoperators/vectordotter.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"

inline void TempLat::VectorDotterTester::Test(TempLat::TDDAssertion &tdd)
{
  {
    static constexpr size_t NDim = 2;
    const ptrdiff_t nGrid = 32, nGhost = 2;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    SpatialCoordinate<NDim> x(toolBox);

    Field<NDim, double> fieldX("fieldX", toolBox);

    fieldX = dot(x, x);

    std::cout << "dot operation: " << dot(x, x).toString() << std::endl;

    const auto fieldX_view = fieldX.getLocalNDHostView();

    bool correct = true;
    for (ptrdiff_t i = 0; i < nGrid; ++i) {
      for (ptrdiff_t j = 0; j < nGrid; ++j) {
        const ptrdiff_t x_val = i > nGrid / 2 ? i - nGrid : i;
        const ptrdiff_t y_val = j > nGrid / 2 ? j - nGrid : j;
        correct &= (fieldX_view(i, j) == x_val * x_val + y_val * y_val);
      }
    }

    tdd.verify(correct);
  }
  {
    static constexpr size_t NDim = 2;
    const ptrdiff_t nGrid = 32, nGhost = 1;

    auto toolBox = MemoryToolBox<NDim>::makeShared(nGrid, nGhost);

    SpatialCoordinate<NDim> x(toolBox);

    Field<NDim, double> fieldX("fieldX", toolBox);

    fieldX = dot(x, x);

    const auto fieldX_view = fieldX.getLocalNDHostView();

    bool correct = true;
    for (ptrdiff_t i = 0; i < nGrid; ++i) {
      for (ptrdiff_t j = 0; j < nGrid; ++j) {
        const ptrdiff_t x_val = i > nGrid / 2 ? i - nGrid : i;
        const ptrdiff_t y_val = j > nGrid / 2 ? j - nGrid : j;
        correct &= (fieldX_view(i, j) == x_val * x_val + y_val * y_val);
      }
    }

    tdd.verify(correct);
  }
}

#endif
