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
    for (ptrdiff_t i = 0; i < fieldX_view.extent(0); ++i) {
      for (ptrdiff_t j = 0; j < fieldX_view.extent(1); ++j) {
        const ptrdiff_t x_val = x.vectorGet(0, nGhost + i, nGhost + j);
        const ptrdiff_t y_val = x.vectorGet(1, nGhost + i, nGhost + j);
        correct &= (fieldX_view(i, j) == x_val * x_val + y_val * y_val);
        if (!(fieldX_view(i, j) == x_val * x_val + y_val * y_val)) {
          std::cout << "Mismatch at (" << i << ", " << j << "): fieldX_view = " << fieldX_view(i, j)
                    << ", expected = " << (x_val * x_val + y_val * y_val) << std::endl;
        }
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
    for (ptrdiff_t i = 0; i < fieldX_view.extent(0); ++i) {
      for (ptrdiff_t j = 0; j < fieldX_view.extent(1); ++j) {
        const ptrdiff_t x_val = x.vectorGet(0, nGhost + i, nGhost + j);
        const ptrdiff_t y_val = x.vectorGet(1, nGhost + i, nGhost + j);
        correct &= (fieldX_view(i, j) == x_val * x_val + y_val * y_val);
      }
    }

    tdd.verify(correct);
  }
}

#endif
