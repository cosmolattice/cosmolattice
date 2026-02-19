#ifndef TEMPLAT_UTIL_POWR_H
#define TEMPLAT_UTIL_POWR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /**
   * @brief A compile-time evaluatable power function for whole number exponents.
   * This implementation generates some rather efficient instructions, see (https://godbolt.org/z/vT56bb1nx).
   *
   * @tparam n Exponent of type int
   * @tparam RF Type of argument
   * @param x Argument
   * @return x^n
   */
  template <ptrdiff_t n, typename NumberType>
    requires requires(NumberType x) {
      x * x;
      static_cast<NumberType>(1) / x;
    }
  constexpr DEVICE_INLINE_FUNCTION NumberType powr(const NumberType x)
  {
    if constexpr (n == 0)
      return static_cast<NumberType>(1);
    else if constexpr (n < 0)
      return static_cast<NumberType>(1) / powr<-n, NumberType>(x);
    else if constexpr (n == 1)
      return x;
    else if constexpr (n % 2 == 0)
      return powr<n / 2>(x) * powr<n / 2>(x);
    else
      return powr<n / 2>(x) * powr<n / 2>(x) * x;
  }
} // namespace TempLat

#endif
