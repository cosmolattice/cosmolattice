#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASEVAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief concept to detect if the eval method has been defined in a given class.
   *
   * Unit test: ctest -R test-haseval
   **/
  template <typename U, typename... IDX>
  concept HasEval = requires(std::decay_t<U> obj, IDX... idx) { obj.eval(idx...); };

#ifdef TEMPLATTEST
  struct HasEvalTester {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
