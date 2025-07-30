
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/fft/types/fftmallocfree.h"
#include "TempLat/fft/types/fftmallocfree_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::FFTMallocFree<double>> test;
#ifndef NOFFTFLOAT
  TempLat::TDDContainer<TempLat::FFTMallocFree<float>> test2;
#endif
} // namespace
