
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025
#include "TempLat/lattice/ghostcells/ghostupdater.h"

namespace
{
  TempLat::TDDContainer<TempLat::GhostUpdater<1>> test1;
  TempLat::TDDContainer<TempLat::GhostUpdater<2>> test2;
  TempLat::TDDContainer<TempLat::GhostUpdater<3>> test3;
  TempLat::TDDContainer<TempLat::GhostUpdater<4>> test4;
  TempLat::TDDContainer<TempLat::GhostUpdater<5>> test5;
  TempLat::TDDContainer<TempLat::GhostUpdater<6>> test6;
} // namespace
