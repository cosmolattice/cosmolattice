#ifndef TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_ONETYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_ONETYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which represents one. Attempt to simplify derivative expressions.
   *
   * Unit test: ctest -R test-zerotype
   **/
  struct OneType {
    static std::string toString() { return "(OneType)1"; }
    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION static constexpr auto eval(const IDX &...i)
    {
      return 1;
    }
    static constexpr int value = 1;
  };
} // namespace TempLat

#endif
