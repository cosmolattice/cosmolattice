
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/fft/external/fftw/fftwplanholder.h"
#include "TempLat/fft/external/fftw/fftwplanholder_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::FFTWPlanHolder<3, float>> test;
  TempLat::TDDContainer<TempLat::FFTWPlanHolder<3, double>> test2;
} // namespace
