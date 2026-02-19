#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDERIVMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASDERIVMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, originally by  Wessel Valkenburg,  Year: 2019

#include <type_traits>
#include <cstddef>

namespace TempLat
{
  /** @brief A concept which finds out whether a method has  derivatives implemented.
   *
   **/
  template <class T>
  concept HasDerivMethod = requires(std::decay_t<T> t, ptrdiff_t idx) { t.d(idx); };

  /** @brief a mini tester class... */

} // namespace TempLat

#endif
