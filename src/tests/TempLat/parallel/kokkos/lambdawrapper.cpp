//
// Created by franz on 6/4/25.
//

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

// The include structure of the library is quite messed up. there are definitiely some circular dependencies.
// Also the test code is by default mangled... this is a bit of a hack to get around that.
#define KOKKOS_SOURCE
#include "TempLat/parallel/kokkos/lambdawrapper.h"
#include "TempLat/parallel/kokkos/lambdawrapper_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::KokkosNDLambdaWrapperTester<1>> test_1;
  TempLat::TDDContainer<TempLat::KokkosNDLambdaWrapperTester<2>> test_2;
  TempLat::TDDContainer<TempLat::KokkosNDLambdaWrapperTester<3>> test_3;
  TempLat::TDDContainer<TempLat::KokkosNDLambdaWrapperTester<4>> test_4;
  TempLat::TDDContainer<TempLat::KokkosNDLambdaWrapperTester<5>> test_5;
  TempLat::TDDContainer<TempLat::KokkosNDLambdaWrapperTester<6>> test_6;
} // namespace
