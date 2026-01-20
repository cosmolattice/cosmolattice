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
  /** \brief concept to detect if the haseval method has been defined.
   *
   * Unit test: make test-haseval
   **/
  template <class T>
  concept HasEval = requires(std::decay_t<T> t, ptrdiff_t idx) { t.eval(idx); };

  struct HasEvalTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
