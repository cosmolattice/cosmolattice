#ifndef TEMPLAT_LATTICE_ALGEBRA_HASSTRINGMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HASSTRINGMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2026

#include <type_traits>

#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  /** @brief A concept which determines at compile time whether an object has a method `get`.
   * See HasGetMethod.
   * Unit test: ctest -R test-hasgetmethod
   **/
  template <class T>
  concept HasStringMethod = requires(std::decay_t<T> t) { t.toString(); };

  template <class T>
  concept HasNoStringMethod = !HasStringMethod<T>;

  template <int N, class T>
  concept HasStringMethodIndexed = requires(std::decay_t<T> t, Tag<N> tag) { t.toString(tag); };

  template <int N, class T>
  concept HasNoStringMethodIndexed = !HasStringMethodIndexed<N, T>;
} // namespace TempLat

#endif
