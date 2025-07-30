#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"

// #include "TempLat/lattice/field/fieldshiftedviewdetection.h"
namespace TempLat
{
  /** \brief A concept which determines at compile time whether an object has a method `confirmGhostsUpToDate`.
   * See HasGetMethod.
   * Unit test: make test-hasgetmethod
   **/
  template <class T>
  concept HasGhostMethod = requires(T t) { t.confirmGhostsUpToDate(); };

  template <int N, class T>
  concept HasGhostMethodIndexed = requires(T t, Tag<N> tag) { t.confirmGhostsUpToDate(tag); };

  /** \brief a mini tester class... */
  struct HasGhostMethodTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
