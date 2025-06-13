
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/memory/memoryblock.h"

namespace
{
  TempLat::TDDContainer<TempLat::MemoryBlock<3, double>> test;
#ifndef NOFFTFLOAT
  TempLat::TDDContainer<TempLat::MemoryBlock<3, float>> test2;
#endif
} // namespace
