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
#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/parallel/kokkos/kokkos_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::KokkosTest> test;
}
