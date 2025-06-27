#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_TEST_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_LAPLACIANLOCAL_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"
#include "TempLat/lattice/algebra/coordinates/spatialcoordinate.h"

template <size_t NDim> inline void TempLat::LatticeLaplacianTester<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  constexpr size_t nd = 3;
  const ptrdiff_t nGrid = 2, nGhost = 1;

  auto show_field = [](const auto &field) {
    auto view = field.getFullNDHostView();

    size_t total_size = 1;
    std::array<size_t, nd> extents;
    for (uint i = 0; i < nd; ++i) {
      extents[i] = view.extent(i);
      total_size *= extents[i];
    }

    std::array<size_t, nd> cIdx{};
    for (size_t i = 0; i < total_size; ++i) {
      // Linear index to cartesian index
      size_t lsize = 1;
      size_t remainder = i;
      for (size_t j = 0; j < nd; ++j) {
        lsize = extents[nd - 1 - j];
        cIdx[nd - 1 - j] = remainder % lsize;
        remainder = (remainder - cIdx[nd - 1 - j]) / extents[nd - 1 - j];
      }
      std::cout << "View(";
      for (uint l = 0; l < nd; ++l) {
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

  Field<nd, double> sc2("SC2", toolBox);
  sc2 = getVectorComponent(x, 1);

  show_field(sc1);
  sc1.updateGhosts();
  show_field(sc1);
  sc2.updateGhosts();

  /*

  auto fgSC = LLLatLapl(sc1);

  bool OK = true;

  // say <<  dum(16,-15,14) ;
  auto it = toolBox->itX();

  for (it.begin(); it.end(); ++it) {
    auto coord = toolBox->getCoordConfiguration(it());
    if (coord[0] != 16 && coord[0] != -15) { // coordinates range from -15 to 16.
      OK = OK && fgSC.get(it()) == 0;        // 0 except at the boundary, where it jumps
    } else if (coord[0] == 16) {
      OK = OK && fgSC.get(it()) == -32; // Go accross the boundary need to get (-15-2* 16+15) == -32
    } else if (coord[0] == -15)
      OK = OK && fgSC.get(it()) == 32; // Go accross the boundary need to get (16+2*15-14) == 32
  }

  tdd.verify(OK);

  auto fgSC2 = LLLatLapl(sc1 * sc1);

  OK = true;

  for (it.begin(); it.end(); ++it) {
    auto coord = toolBox->getCoordConfiguration(it());
    if (coord[0] != 16 && coord[0] != -15) { // coordinates range from -15 to 16.
      OK = OK && fgSC2.get(it()) == 2;       // 2 except at the boundary, where it jumps
    } else if (coord[0] == 16) {
      // say <<
      OK = OK && fgSC2.get(it()) == -62; // Go accross the boundary need to get ((-15)**2-2* (16)**2+(15)**2) == -62
    } else if (coord[0] == -15)
      OK = OK && fgSC2.get(it()) == 2; // Go accross the boundary need to get ((16)**2-2*(-15)**2-(14)**2) == 2
  }
  //

  tdd.verify(OK);

  //
  auto fgSC3 = LLLatLapl(sc1 * sc2);
  //
  OK = true;

  for (it.begin(); it.end(); ++it) {
    auto coord = toolBox->getCoordConfiguration(it());
    if (coord[0] != 16 && coord[0] != -15 && coord[0] != 16 && coord[0] != -15 && coord[1] != 16 &&
        coord[1] != -15) {             // coordinates range from -15 to 16.
      OK = OK && fgSC3.get(it()) == 0; // 2 except at the boundary, where it jumps
    }

    // Don't check the boundary case as it is becoming noying. But please feel free to if you have any doubts.
  }

  tdd.verify(OK);

  //
  // laplacian of x^2 y^2: 2 * (y=2)^2 + (x=1)^1 * 2 = 10
  auto fgSC4 = LLLatLapl(pow<2>(sc1 * sc2));

  Field<3, double> dum("dummy", toolBox);

  dum = fgSC4;
  bool test;

  // auto res = dum(test, 14, 12, 14); // test check to see if the coordinate is on this process or not.

  // if (test) tdd.verify(res == 680);

  //   laplacian of x^2 + 2 x y + y^2 = 4
  auto fgSC5 = LLLatLapl(pow<2>(sc1 + sc2));
  //
  OK = true;

  for (it.begin(); it.end(); ++it) {
    auto coord = toolBox->getCoordConfiguration(it());
    if (coord[0] != 16 && coord[0] != -15 && coord[0] != 16 && coord[0] != -15 && coord[1] != 16 &&
        coord[1] != -15) {             // coordinates range from -15 to 16.
      OK = OK && fgSC5.get(it()) == 4; // 2 except at the boundary, where it jumps
    }
    // Don't check the boundary case as it is becoming noying. But please feel free to if you have any doubts.
  }
  tdd.verify(OK);
  */
}

#endif
