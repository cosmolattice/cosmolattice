
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/field/views/fieldviewconfig.h"
#include "TempLat/lattice/field/views/fieldviewconfig_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::ConfigViewTester<1, double>> test1;
  TempLat::TDDContainer<TempLat::ConfigViewTester<2, double>> test2;
  TempLat::TDDContainer<TempLat::ConfigViewTester<3, double>> test3;
  TempLat::TDDContainer<TempLat::ConfigViewTester<4, double>> test4;
  TempLat::TDDContainer<TempLat::ConfigViewTester<1, float>> test1f;
  TempLat::TDDContainer<TempLat::ConfigViewTester<2, float>> test2f;
  TempLat::TDDContainer<TempLat::ConfigViewTester<3, float>> test3f;
  TempLat::TDDContainer<TempLat::ConfigViewTester<4, float>> test4f;
} // namespace
