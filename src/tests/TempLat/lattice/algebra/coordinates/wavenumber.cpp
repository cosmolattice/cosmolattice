/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/coordinates/wavenumber.h"
#include "TempLat/lattice/algebra/coordinates/wavenumber_test.h"

namespace
{
  // TODO: FFT FAILURES ON D=1 !!! (MPI related.)
  // TempLat::TDDContainer<TempLat::WaveNumberTester<1>> test1;
  TempLat::TDDContainer<TempLat::WaveNumberTester<2>> test2;
  TempLat::TDDContainer<TempLat::WaveNumberTester<3>> test3;
  TempLat::TDDContainer<TempLat::WaveNumberTester<4>> test4;
  TempLat::TDDContainer<TempLat::WaveNumberTester<5>> test5;
} // namespace
