#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** @brief A concept which can be used to detect whether something is a composite object or not.
   *
   *
   * Unit test: ctest -R test-hasstaticgetter
   **/
  template <typename T>
  concept HasStaticGetter = requires(T t, Tag<0> tag) { T::Getter::get(t, tag); };

} // namespace TempLat

#endif
