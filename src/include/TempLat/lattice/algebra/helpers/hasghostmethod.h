#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGHOSTMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2026

#include "TempLat/util/rangeiteration/tag.h"

// #include "TempLat/lattice/field/fieldshiftedviewdetection.h"
namespace TempLat
{
  /** @brief A concept which determines at compile time whether an object has a method `confirmGhostsUpToDate`.
   * See HasGetMethod.
   * Unit test: ctest -R test-hasgetmethod
   **/
  template <class T>
  concept HasGhostMethod = requires(std::decay_t<T> t) { t.confirmGhostsUpToDate(); };

  template <int N, class T>
  concept HasGhostMethodIndexed = requires(std::decay_t<T> t, Tag<N> tag) { t.confirmGhostsUpToDate(tag); };

  template <class T>
  concept HasGhostMethodDirectIndexed = requires(std::decay_t<T> t, ptrdiff_t i) { t.confirmGhostsUpToDate(i); };

  template <int N, class T>
  concept HasGhostMethodElement = requires(std::decay_t<T> t, Tag<N> tag) { t(tag).confirmGhostsUpToDate(); };

  template <class T>
  concept HasGhostMethodDirectElement = requires(std::decay_t<T> t, ptrdiff_t i) { t(i).confirmGhostsUpToDate(); };

} // namespace TempLat

#endif
