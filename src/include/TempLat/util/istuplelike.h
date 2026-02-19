#ifndef TEMPLAT_UTIL_ISTUPLELIKE_H
#define TEMPLAT_UTIL_ISTUPLELIKE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <tuple>
#include <type_traits>

namespace TempLat
{
  /** @brief A class which tests whether or not a class behaves like a tuple.
   *
   *
   * Unit test: ctest -R test-istuplelike
   **/
  template <class, class = std::void_t<>> struct IsTupleLike : std::false_type {
  };

  template <class T> struct IsTupleLike<T, std::void_t<typename std::tuple_size<T>::type>> : std::true_type {
  };

} // namespace TempLat

#endif
