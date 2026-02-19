#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISTempLatGETTABLE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISTempLatGETTABLE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** @brief A concept which which checks for the existence of getComp-
   *
   *
   * Unit test: ctest -R test-istemplatgettable
   **/
  template <int N, class T>
  concept IsTempLatGettable = requires(std::decay_t<T> t, Tag<N> tag) { t.getComp(tag); };
} // namespace TempLat

#endif
