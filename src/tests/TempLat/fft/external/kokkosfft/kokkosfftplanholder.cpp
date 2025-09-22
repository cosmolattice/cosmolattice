 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifndef NOPFFT
#include "TempLat/fft/external/pfft/pfftplanholder.h"
namespace {
    TempLat::TDDContainer<TempLat::PFFTPlanHolder<float>> test;
    TempLat::TDDContainer<TempLat::PFFTPlanHolder<double>> test2;
}
#endif

