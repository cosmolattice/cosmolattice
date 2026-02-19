/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare.h"
#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare_test.h"

namespace
{
  // TODO 1-Dim fails due to fourier transform - FIND OUT WHY
  // TempLat::TDDContainer<TempLat::NormGradientSquareTester<1>> test1;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<2>> test2;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<3>> test3;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<4>> test4;
  TempLat::TDDContainer<TempLat::NormGradientSquareTester<5>> test5;
} // namespace
