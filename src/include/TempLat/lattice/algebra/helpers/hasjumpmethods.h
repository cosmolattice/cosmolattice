#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASJUMPMETHODS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/jumpsholder.h"

namespace TempLat
{
  template <size_t NDim, class T>
  concept HasJumpMethods = requires(std::decay_t<T> t) {
    { t.getJumps() } -> std::convertible_to<JumpsHolder<NDim>>;
  };

  /** \brief a mini tester class... */
  struct HasJumpMethodsTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
