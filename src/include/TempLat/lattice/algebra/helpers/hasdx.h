#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDX_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDX_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief
   * concept to detect whether something has a getDx() method or not.
   *
   * Unit test: make test-hasdx
   **/

  template <class T>
  concept HasDx = requires(std::decay_t<T> t) {
    { t.getDx() } -> std::convertible_to<double>;
  };

  struct HasDxTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
