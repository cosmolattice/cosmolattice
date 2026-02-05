/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/spatialderivatives/latticeforwardgradient.h"
#include "TempLat/lattice/algebra/spatialderivatives/latticeforwardgradient_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<1>> test1;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<2>> test2;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<3>> test3;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<4>> test4;
  TempLat::TDDContainer<TempLat::LatticeForwardGradientTester<5>> test5;
} // namespace
