#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETEVAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  template <typename U, typename... IDX>
  concept HasGetEval = requires(std::decay_t<U> obj, IDX... idx) { obj.getEval(idx...); };

  struct HasGetEvalTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
