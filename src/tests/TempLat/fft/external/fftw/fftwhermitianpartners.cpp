
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"
#include "TempLat/fft/external/fftw/fftwhermitianpartners_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::FFTWHermitianPartners<1>> test1;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartners<2>> test2;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartners<3>> test3;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartners<4>> test4;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartners<5>> test5;
  TempLat::TDDContainer<TempLat::FFTWHermitianPartners<6>> test6;
} // namespace
