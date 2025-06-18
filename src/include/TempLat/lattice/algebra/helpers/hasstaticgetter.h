#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSTATICGETTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  template <typename T>
  concept HasStaticGet = requires(T t, Tag<0> tag) { T::Getter::get(t, tag); };

  struct HasStaticGetterDummy {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasstaticgetter_test.h"
#endif

#endif
