#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDOWENEEDGHOSTS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDOWENEEDGHOSTS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <type_traits>

#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** @brief concept to see if the doWeNeedGhost method is defined.
   *
   * Unit test: ctest -R test-hasdoweneedghosts
   **/
  template <class T>
  concept HasDoWeNeedGhostsMethod = requires(std::decay_t<T> t) { t.doWeNeedGhosts(); };

  template <int N, class T>
  concept HasDoWeNeedGhostsMethodIndexed = requires(std::decay_t<T> t, Tag<N> tag) { t.doWeNeedGhosts(tag); };

  template <class T>
  concept HasDoWeNeedGhostsMethodIndexedDyn = requires(std::decay_t<T> t, ptrdiff_t idx) { t.doWeNeedGhosts(idx); };

} // namespace TempLat

#endif
