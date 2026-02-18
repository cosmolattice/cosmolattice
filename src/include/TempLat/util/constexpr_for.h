#ifndef TEMPLAT_UTIL_CONSTEXPR_FOR_H
#define TEMPLAT_UTIL_CONSTEXPR_FOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include <type_traits>
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  /**
   * @brief A compile-time for loop, which calls the lambda f of signature void(integer) for each index.
   */
  template <auto Start, auto End, class F> DEVICE_FORCEINLINE_FUNCTION constexpr void constexpr_for(F &&f)
  {
    if constexpr (Start < End) {
      f(Tag<Start>{});
      constexpr_for<Start + 1, End>(f);
    }
  }
} // namespace TempLat

#endif