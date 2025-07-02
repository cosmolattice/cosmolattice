#ifndef TEMPLAT_UTIL_ISARRAY_H
#define TEMPLAT_UTIL_ISARRAY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/tdd/tdd.h"
#include <type_traits>

namespace TempLat
{
  /** \brief Checks if a type is an array of a specific size,
   * i.e. it has the size() NDim and an operator[].
   *
   **/
  template <typename T, size_t NDim>
  concept IsArray = std::is_same_v<std::array<typename T::value_type, NDim>, T> ||
                    std::is_same_v<Kokkos::Array<typename T::value_type, NDim>, T>;

  struct IsArrayTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
