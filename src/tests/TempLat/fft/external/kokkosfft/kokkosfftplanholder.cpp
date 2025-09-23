
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifdef KOKKOS_FFT
#include "TempLat/fft/external/kokkosfft/kokkosfftplanholder.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftplanholder_test.h"
namespace
{
  TempLat::TDDContainer<TempLat::KokkosFFTPlanHolder<float>> test;
  TempLat::TDDContainer<TempLat::KokkosFFTPlanHolder<double>> test2;
} // namespace
#endif
