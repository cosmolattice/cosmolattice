#ifndef TEMPLAT_UTIL_ALMOSTEQUAL_H
#define TEMPLAT_UTIL_ALMOSTEQUAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <limits> /** to get epsilons for various types */

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief we are comparing computed floats, so allow for some epsilon */
  template <typename T1, typename T2, typename T3 = T1>
    requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>)
  DEVICE_FUNCTION bool AlmostEqual(const T1 &a, const T2 &b,
                                   const T3 &epsilon = std::sqrt(std::numeric_limits<T3>::epsilon()))
  {
    if (std::isnan(a) || std::isnan(b)) return false;
    if (a == b) return true;
    if (std::abs(a) < epsilon)
      return std::abs(b) < epsilon;
    else if (std::abs(b) < epsilon)
      return std::abs(a) < epsilon;
    else
      // I added a test of absolute difference to catch the edge-case where both values are effectively zero.
      return std::abs(a / b - 1) < epsilon || std::abs(a - b) < std::numeric_limits<T3>::epsilon() * 2;
  };

  /** @brief overload for complex values.  */
  template <typename T>
  DEVICE_FUNCTION bool AlmostEqual(const complex<T> &a, const complex<T> &b,
                                   const T epsilon = std::sqrt(std::numeric_limits<T>::epsilon()))
  {
    return AlmostEqual(a.real(), b.real(), epsilon) && AlmostEqual(a.imag(), b.imag(), epsilon);
  };

  /** @brief overload for arrays */
  template <typename T, size_t N>
  DEVICE_FUNCTION bool AlmostEqual(const std::array<T, N> &a, const std::array<T, N> &b,
                                   const T epsilon = std::sqrt(std::numeric_limits<T>::epsilon()))
  {
    bool result = true;
    for (ptrdiff_t i = 0; i < (ptrdiff_t)N; ++i) {
      result = result && AlmostEqual(a[i], b[i], epsilon);
    }
    return result;
  };

  /** @brief A class for testing AlmostEqual
   *
   *
   * Unit test: ctest -R test-almostequal
   */
} // namespace TempLat

#endif
