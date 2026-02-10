#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** @brief A concept which can be used to detect whether something is a composite object or not.
   *
   *
   * Unit test: make test-hasstaticgetter
   **/
  template <typename T>
  concept HasStaticGetter = requires(T t, Tag<0> tag) { T::Getter::get(t, tag); };

#ifdef TEMPLATTEST
  struct HasStaticGetterDummy {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
