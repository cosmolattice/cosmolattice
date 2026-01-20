/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/devices/kokkos/kokkos_iteration.h"
#include "TempLat/parallel/devices/kokkos/kokkos_iteration_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::KokkosIterationTester<1>> test_1;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<2>> test_2;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<3>> test_3;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<4>> test_4;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<5>> test_5;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<6>> test_6;
} // namespace
