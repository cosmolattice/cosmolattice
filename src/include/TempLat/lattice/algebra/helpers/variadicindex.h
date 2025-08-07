#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_VARIADICINDEX_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_VARIADICINDEX_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /**
   * @brief A concept that checks if the given types are integral and the number of types matches the given dimension.
   */
  template <size_t NDim, typename... IDX>
  concept VariadicNDIndex = requires {
    requires(sizeof...(IDX) == NDim);
    requires((std::is_integral_v<std::decay_t<IDX>> && ...));
  };

  /**
   * @brief A concept that checks if the given types are integral and the number of types is at least 1.
   */
  template <typename... IDX>
  concept VariadicIndex = requires {
    requires(sizeof...(IDX) > 0);
    requires((std::is_integral_v<std::decay_t<IDX>> && ...));
  };

  class VariadicIndexTester
  {
  public:
    template <typename Assertion> static inline void Test(Assertion &tdd);
  };
} // namespace TempLat

#endif
