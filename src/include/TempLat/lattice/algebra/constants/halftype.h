#ifndef TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_HALFTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_HALFTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which implement 1/2 as a type. Useful for squareroots.
   *
   * Unit test: ctest -R test-halftype
   **/
  struct HalfType {
    static std::string toString() { return "(HalfType)1/2"; }
    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION static constexpr auto eval(const IDX &...i)
    {
      return 0.5f;
    }
    static constexpr float value = 0.5f;
  };
} // namespace TempLat

#endif
