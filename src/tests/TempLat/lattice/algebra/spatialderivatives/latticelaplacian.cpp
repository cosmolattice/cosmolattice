/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticelaplacian_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<1>> test1;
  // TODO 2-Dim fails due to fourier transform - FIND OUT WHY
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<2>> test2;
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<3>> test3;
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<4>> test4;
  TempLat::TDDContainer<TempLat::LatticeLaplacianTester<5>> test5;
} // namespace
