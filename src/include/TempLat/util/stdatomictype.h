#ifndef TEMPLAT_UTIL_STDATOMICTYPE_H
#define TEMPLAT_UTIL_STDATOMICTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2025

#include <type_traits>

namespace TempLat
{

  /** @brief A SFINAE class to get the base type of a composite container.
   *
   * Unit test: ctest -R test-stdatomictype
   **/

  // Base case: T is not a container
  template <typename T, typename = void>
  struct std_atomic_type {
    using type = T;
  };

  // Specialization: T has a nested value_type (i.e. is a container)
  template <typename T>
  struct std_atomic_type<T, std::void_t<typename T::value_type>> {
    using type = typename std_atomic_type<typename T::value_type>::type;
  };

} // namespace TempLat

#endif
