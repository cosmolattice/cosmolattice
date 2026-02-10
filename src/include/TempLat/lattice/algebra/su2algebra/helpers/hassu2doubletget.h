#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{

  /** @brief A concept which checks whether the object has a su2doubletget method or not.
   *
   * Unit test: make test-hassu2doubletget
   **/
  template <typename T>
  concept HasSU2DoubletGet = requires(T t, Tag<0> tag) { t.SU2DoubletGet(tag); };

  struct HasSU2DoubletGetTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
