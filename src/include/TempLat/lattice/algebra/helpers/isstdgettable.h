#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISSTDGETTABLE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISSTDGETTABLE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A concept which checks compatibility with std::get.
   *
   * Unit test: ctest -R test-isstdgettable
   **/
  template <int N, class T>
  concept IsSTDGettable = requires(T t) { std::get<N>(t); };
} // namespace TempLat

#endif
