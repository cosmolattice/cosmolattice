#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_IsVariadicIndex_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_IsVariadicIndex_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include <cstddef>
#include <type_traits>

namespace TempLat
{
  /**
   * @brief A concept that checks if the given types are integral and the number of types matches the given dimension.
   */
  template <size_t NDim, typename... IDX>
  concept IsVariadicNDIndex = requires {
    requires(sizeof...(IDX) == NDim);
    requires((std::is_integral_v<std::decay_t<IDX>> && ...));
  };

  /**
   * @brief A concept that checks if the given types are integral and the number of types is at least 1.
   */
  template <typename... IDX>
  concept IsVariadicIndex = requires {
    requires(sizeof...(IDX) > 0);
    requires((std::is_integral_v<std::decay_t<IDX>> && ...));
  };

  class IsVariadicIndexTester
  {
  public:
    template <typename Assertion> static inline void Test(Assertion &tdd);
  };
} // namespace TempLat

#endif
