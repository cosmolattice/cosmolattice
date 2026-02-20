#ifndef TEMPLAT_UTIL_FLATTENSTD_H
#define TEMPLAT_UTIL_FLATTENSTD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Year: 2025

#include "TempLat/util/stdatomictype.h"

#include <vector>

namespace TempLat
{

  /** @brief A class which flattens a std vector.
   *
   * Unit test: ctest -R test-flattenstd
   **/

  // Trait to detect std::vector
  template <typename T>
  struct is_std_vector : std::false_type {};

  template <typename T, typename A>
  struct is_std_vector<std::vector<T, A>> : std::true_type {};

  // Recursive flatten function for atomic types (leaf)
  template <typename T>
  typename std::enable_if<!is_std_vector<T>::value>::type flatten_helper(const T &elem, std::vector<T> &flat)
  {
    flat.push_back(elem);
  }

  // Recursive flatten function for vectors
  template <typename T>
  typename std::enable_if<is_std_vector<T>::value>::type
  flatten_helper(const T &nested, std::vector<typename std_atomic_type<T>::type> &flat)
  {
    for (const auto &elem : nested) {
      flatten_helper(elem, flat);
    }
  }

  // Helper wrapper
  template <typename NestedVec>
  std::vector<typename std_atomic_type<NestedVec>::type> flatten(const NestedVec &nested)
  {
    using AtomicT = typename std_atomic_type<NestedVec>::type;
    std::vector<AtomicT> flat;
    flatten_helper(nested, flat);
    return flat;
  }

} // namespace TempLat

#endif
