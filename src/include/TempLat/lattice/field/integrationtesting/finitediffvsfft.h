#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_FINITEDIFFVSFFT_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_FINITEDIFFVSFFT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/cosmolattice.h"

namespace TempLat
{
  /** @brief Possibly the ultimate test for the implementations of
   *   various spatial derivatives: compare finite difference results
   *   with FFT results.
   *
   * Unit test: ctest -R test-finitediffvsfft
   **/
} // namespace TempLat

#endif
