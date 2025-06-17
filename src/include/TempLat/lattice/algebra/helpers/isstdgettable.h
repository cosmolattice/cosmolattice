#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISSTDGETTABLE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISSTDGETTABLE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** \brief A concept which checks compatibility with std::get.
   *
   * Unit test: make test-isstdgettable
   **/
  template <int N, class T>
  concept IsSTDGettable = requires(T t) { std::get<N>(t); };

  struct IsSTDGettableTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/isstdgettable_test.h"
#endif

#endif
