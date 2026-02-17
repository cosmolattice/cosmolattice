
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifdef HAVE_PFFT
#include "TempLat/fft/external/pfft/pfftplanholder.h"
namespace
{
  TempLat::TDDContainer<TempLat::PFFTPlanHolder<3, float>> test;
  TempLat::TDDContainer<TempLat::PFFTPlanHolder<3, double>> test2;
} // namespace
#endif
