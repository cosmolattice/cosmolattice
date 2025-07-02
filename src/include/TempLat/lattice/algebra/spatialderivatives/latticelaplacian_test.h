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
  const ptrdiff_t nGrid = 32, nGhost = 1;

  auto get_cIdx = [](const auto idx, const auto total_size, const auto extents) {
    std::array<size_t, nd> cIdx{};
    // Linear index to cartesian index
    size_t lsize = 1;
    size_t remainder = idx;
    for (size_t j = 0; j < nd; ++j) {
      lsize = extents[nd - 1 - j];
      cIdx[nd - 1 - j] = remainder % lsize;
      remainder = (remainder - cIdx[nd - 1 - j]) / extents[nd - 1 - j];
    }
    return cIdx;
  };

  auto show_field = [&](const auto &field) {
    auto view = field.getFullNDHostView();

    size_t total_size = 1;
    std::array<size_t, nd> extents;
    for (size_t i = 0; i < nd; ++i) {
      extents[i] = view.extent(i);
      total_size *= extents[i];
    }

    for (size_t i = 0; i < total_size; ++i) {
      std::array<size_t, nd> cIdx = get_cIdx(i, total_size, extents);
      std::cout << "View(";
      for (size_t l = 0; l < nd; ++l) {
        std::cout << cIdx[l];
        if (l != nd - 1) std::cout << ", ";
      }
      std::apply([&](const auto &...args) { std::cout << ") = " << view(args...) << std::endl; }, cIdx);
    }
  };

  auto toolBox = MemoryToolBox<nd>::makeShared(nGrid, nGhost);
  SpatialCoordinate x(toolBox);
  toolBox->setVerbose();

  Field<nd, double> sc1("SC1", toolBox);
  sc1 = getVectorComponent(x, 0);
  sc1.updateGhosts();
  show_field(sc1);

  Field<nd, double> sc2("SC2", toolBox);
  sc2 = getVectorComponent(x, 1);
  sc2.updateGhosts();

  Field<nd, double> result_field("SC2", toolBox);
  auto result_view = result_field.getLocalNDHostView();
  size_t total_size = 1;
  std::array<size_t, nd> extents;
  for (size_t i = 0; i < nd; ++i) {
    extents[i] = result_view.extent(i);
    total_size *= extents[i];
  }

  result_field = LatLapl<nd>(sc1);
  result_view = result_field.getLocalNDHostView();
  bool OK = true;
  for (size_t i = 0; i < total_size; ++i) {
    const auto coord = get_cIdx(i, total_size, extents);
    // It's a linear function, except at the boundaries, where it jumps
    std::apply(
        [&](const auto... idx) {
          if (coord[0] != 0 && coord[0] != nGrid - 1) {
            // 0 except at the boundary, where it jumps
            OK = OK && result_view(idx...) == 0;
            if (!(result_view(idx...) == 0))
              std::cout << "bulk fail: " << result_view(idx...) << ", expected: " << 0 << std::endl;
          } else if (coord[0] == nGrid - 1) {
            OK = OK && result_view(idx...) == -nGrid;
            if (!(result_view(idx...) == -nGrid))
              std::cout << "b1 fail: " << result_view(idx...) << ", expected: " << -nGrid << std::endl;
          } else if (coord[0] == 0) {
            OK = OK && result_view(idx...) == nGrid;
            if (!(result_view(idx...) == nGrid))
              std::cout << "b2 fail: " << result_view(idx...) << ", expected: " << nGrid << std::endl;
          }
        },
        coord);
  }
  tdd.verify(OK);

  result_field = LatLapl<nd>(sc1 * sc1);
  result_view = result_field.getLocalNDHostView();
  OK = true;
  for (size_t i = 0; i < total_size; ++i) {
    const auto coord = get_cIdx(i, total_size, extents);
    std::apply(
        [&](const auto... idx) {
          // It's a x^2, except at the boundaries, where it jumps
          if (coord[0] != 0 && coord[0] != nGrid - 1) {
            OK = OK && result_view(idx...) == 2;
            if (!(result_view(idx...) == 2))
              std::cout << "bulk fail: " << result_view(idx...) << ", expected: " << 2 << std::endl;
          } else if (coord[0] == nGrid - 1) {
            OK = OK && result_view(idx...) == (pow<2>(1) - 2 * pow<2>(nGrid) + pow<2>(nGrid - 1));
            if (!(result_view(idx...) == (pow<2>(1) - 2 * pow<2>(nGrid) + pow<2>(nGrid - 1))))
              std::cout << "b1 fail: " << result_view(idx...)
                        << ", expected: " << (pow<2>(1) - 2 * pow<2>(nGrid) + pow<2>(nGrid - 1)) << std::endl;
          } else if (coord[0] == 0) {
            OK = OK && result_view(idx...) == (pow<2>(2) - 2 * pow<2>(1) + pow<2>(nGrid));
            if (!(result_view(idx...) == (pow<2>(2) - 2 * pow<2>(1) + pow<2>(nGrid))))
              std::cout << "b2 fail: " << result_view(idx...)
                        << ", expected: " << (pow<2>(2) - 2 * pow<2>(1) + pow<2>(nGrid)) << std::endl;
          }
        },
        coord);
  }
  tdd.verify(OK);

  result_field = LatLapl<nd>(sc1 * sc2);
  result_view = result_field.getLocalNDHostView();
  OK = true;
  for (size_t i = 0; i < total_size; ++i) {
    const auto coord = get_cIdx(i, total_size, extents);
    std::apply(
        [&](const auto... idx) {
          if (coord[0] != 0 && coord[0] != nGrid - 1 && coord[1] != 0 && coord[1] != nGrid - 1) {
            OK = OK && result_view(idx...) == 0;
            if (!(result_view(idx...) == 0))
              std::cout << "bulk fail: " << result_view(idx...) << ", expected: " << 2 << std::endl;
          }
          // Don't check the boundary case as it is becoming annoying. But please feel free to if you have any doubts.
        },
        coord);
  }
  tdd.verify(OK);

  //   laplacian of x^2 + 2 x y + y^2 = 4
  result_field = LatLapl<nd>(pow<2>(sc1 + sc2));
  result_view = result_field.getLocalNDHostView();
  OK = true;
  for (size_t i = 0; i < total_size; ++i) {
    const auto coord = get_cIdx(i, total_size, extents);
    std::apply(
        [&](const auto... idx) {
          if (coord[0] != 0 && coord[0] != nGrid - 1 && coord[1] != 0 && coord[1] != nGrid - 1) {
            OK = OK && result_view(idx...) == 4;
            if (!(result_view(idx...) == 4))
              std::cout << "bulk fail: " << result_view(idx...) << ", expected: " << 2 << std::endl;
          }
          // Don't check the boundary case as it is becoming annoying. But please feel free to if you have any doubts.
        },
        coord);
  }
  tdd.verify(OK);
}

#endif
