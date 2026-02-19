#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** @brief A concept which determines at compile time whether an object has a method `vectorGet`.
   *
   **/
  template <class U>
  concept HasVectorGetMethod = requires(std::decay_t<U> u) { u.vectorGet(Tag<1>()); };
} // namespace TempLat

#endif
