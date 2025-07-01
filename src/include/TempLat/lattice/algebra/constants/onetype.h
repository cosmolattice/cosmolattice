#ifndef TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_ONETYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_ONETYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  /** \brief A class which represents one. Attempt to simplify derivative expressions.
   *
   * Unit test: make test-zerotype
   **/

  struct OneType {
    //        operator ptrdiff_t() const { return 1; }
    static std::string toString() { return "(OneType)1"; }
    template <std::integral... IDX> KOKKOS_FORCEINLINE_FUNCTION static constexpr double get(const IDX &...)
    {
      return 1;
    }
    static constexpr bool ISCONSTANT = true;
  };

} // namespace TempLat

#endif
