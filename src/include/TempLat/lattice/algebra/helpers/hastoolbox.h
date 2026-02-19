#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASTOOLBOX_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASTOOLBOX_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorytoolbox.h"

namespace TempLat
{
  /** @brief A concept which determines at compile time whether an object has a method `getToolBox`.
   * See HasGetMethod.
   * Unit test: ctest -R test-hastoolbox
   **/
  template <class T>
  concept HasToolBox = requires(std::decay_t<T> t) { t.getToolBox(); };
} // namespace TempLat

#endif
