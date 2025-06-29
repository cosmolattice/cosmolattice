#ifndef TEMPLAT_UTIL_CONSTEXPR_FOR_H
#define TEMPLAT_UTIL_CONSTEXPR_FOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include <type_traits>

namespace TempLat
{
  /**
   * @brief A compile-time for loop, which calls the lambda f of signature void(integer) for each index.
   */
  template <auto Start, auto End, auto Inc, class F> constexpr void constexpr_for(F &&f)
  {
    if constexpr (Start < End) {
      f(std::integral_constant<decltype(Start), Start>{});
      constexpr_for<Start + Inc, End, Inc>(f);
    }
  }
} // namespace TempLat

#endif