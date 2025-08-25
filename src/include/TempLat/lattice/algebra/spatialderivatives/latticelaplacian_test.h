#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"
#include "TempLat/lattice/field/field.h"

template <size_t NDim> inline void TempLat::LatticeLaplacianTester<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  constexpr size_t nd = 2;
  const ptrdiff_t nGrid = 4, nGhost = 1;

  auto show_field = [&](const auto &field) {
    auto view = field.getFullNDHostView();
    std::cout << "Field " << field.to_string() << " :" << std::endl;
    for (size_t i = 0; i < nGrid + 2 * nGhost; ++i) {
      for (size_t j = 0; j < nGrid + 2 * nGhost; ++j) {
        std::cout << std::setw(5) << view(i, j);
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  };

  auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);
  SpatialCoordinate coord(toolBox);
  toolBox->setVerbose();

  Field<nd, double> sc1("SC1", toolBox);
  sc1 = getVectorComponent(coord, 0);
  sc1.updateGhosts();
  show_field(sc1);

  Field<nd, double> sc2("SC2", toolBox);
  sc2 = getVectorComponent(coord, 1);
  sc2.updateGhosts();
  show_field(sc2);

  Field<nd, double> result_field("result", toolBox);
  auto result_view = result_field.getLocalNDHostView();

  result_field = LatLapl<nd>(sc1);
  show_field(result_field);
  result_view = result_field.getLocalNDHostView();
  bool OK = true;
  for (size_t i = 0; i < nGrid; ++i) {
    for (size_t j = 0; j < nGrid; ++j) {
      // It's a linear function, except at two points: at nGrid/2 and nGrid/2+1, where it goes from nGrid/2 to
      // -nGrid/2 + 1
      if (i == nGrid / 2) {
        OK = OK && result_view(i, j) == -nGrid;
        if (!(result_view(i, j) == -nGrid))
          std::cout << "b1 fail: " << result_view(i, j) << ", expected: " << -nGrid << std::endl;
      } else if (i == nGrid / 2 + 1) {
        OK = OK && result_view(i, j) == nGrid;
        if (!(result_view(i, j) == nGrid))
          std::cout << "b2 fail: " << result_view(i, j) << ", expected: " << nGrid << std::endl;
      } else {
        // 0 except at the boundary, where it jumps
        OK = OK && result_view(i, j) == 0;
        if (!(result_view(i, j) == 0))
          std::cout << "bulk fail: " << result_view(i, j) << ", expected: " << 0 << std::endl;
      }
    }
  }
  tdd.verify(OK);

  result_field = LatLapl<nd>(sc1 * sc1);
  show_field(result_field);
  result_view = result_field.getLocalNDHostView();
  OK = true;
  for (size_t i = 0; i < nGrid; ++i) {
    for (size_t j = 0; j < nGrid; ++j) {
      // It's a linear function, except at two points: at nGrid/2 and nGrid/2+1, where it goes from nGrid/2 to
      // -nGrid/2 + 1
      if (i == nGrid / 2) {
        const double expected = (pow<2>(nGrid / 2 - 1) - 2 * pow<2>(nGrid / 2) + pow<2>(-nGrid / 2 + 1));
        OK = OK && result_view(i, j) == expected;
        if (!(result_view(i, j) == expected))
          std::cout << "b1 fail: " << result_view(i, j) << ", expected: " << expected << std::endl;
      } else if (i == nGrid / 2 + 1) {
        const double expected = (pow<2>(nGrid / 2) - 2 * pow<2>(-nGrid / 2 + 1) + pow<2>(-nGrid / 2 + 2));
        OK = OK && result_view(i, j) == expected;
        if (!(result_view(i, j) == expected))
          std::cout << "b2 fail: " << result_view(i, j) << ", expected: " << expected << std::endl;
      } else {
        // 0 except at the boundary, where it jumps
        OK = OK && result_view(i, j) == 2;
        if (!(result_view(i, j) == 2))
          std::cout << "bulk fail: " << result_view(i, j) << ", expected: " << 2 << std::endl;
      }
    }
  }
  tdd.verify(OK);

  result_field = LatLapl<nd>(sc1 * sc2);
  show_field(result_field);
  result_view = result_field.getLocalNDHostView();
  OK = true;
  for (int i = 0; i < nGrid; ++i) {
    for (int j = 0; j < nGrid; ++j) {
      const double x = i > nGrid / 2 ? i - nGrid : i;
      const double xm1 = (i - 1) > nGrid / 2 ? (i - 1) - nGrid : (i - 1);
      const double xp1 = (i + 1) > nGrid / 2 ? (i + 1) - nGrid : (i + 1);
      const double y = j > nGrid / 2 ? j - nGrid : j;
      const double ym1 = (j - 1) > nGrid / 2 ? (j - 1) - nGrid : (j - 1);
      const double yp1 = (j + 1) > nGrid / 2 ? (j + 1) - nGrid : (j + 1);
      const double expected = -4 * (x * y) + (xp1 * y + xm1 * y) + (x * yp1 + x * ym1);

      OK = OK && result_view(i, j) == expected;
      if (!(result_view(i, j) == expected))
        std::cout << "bulk fail at (" << i << ", " << j << "): " << result_view(i, j) << ", expected: " << expected
                  << std::endl;
    }
  }
  tdd.verify(OK);

  //   laplacian of x^2 + 2 x y + y^2 = 4
  result_field = LatLapl<nd>(pow<2>(sc1 + sc2));
  show_field(result_field);
  result_view = result_field.getLocalNDHostView();
  OK = true;
  for (int i = 0; i < nGrid; ++i) {
    for (int j = 0; j < nGrid; ++j) {
      const double x = i > nGrid / 2 ? i - nGrid : i;
      const double xm1 = (i - 1) > nGrid / 2 ? (i - 1) - nGrid : (i - 1);
      const double xp1 = (i + 1) > nGrid / 2 ? (i + 1) - nGrid : (i + 1);
      const double y = j > nGrid / 2 ? j - nGrid : j;
      const double ym1 = (j - 1) > nGrid / 2 ? (j - 1) - nGrid : (j - 1);
      const double yp1 = (j + 1) > nGrid / 2 ? (j + 1) - nGrid : (j + 1);
      const double expected =
          -4 * powr<2>(x + y) + (pow<2>(xp1 + y) + pow<2>(xm1 + y)) + (pow<2>(x + yp1) + pow<2>(x + ym1));

      OK = OK && result_view(i, j) == expected;
      if (!(result_view(i, j) == expected))
        std::cout << "bulk fail: " << result_view(i, j) << ", expected: " << expected << std::endl;
    }
  }
  tdd.verify(OK);
}

#endif
