#ifndef TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_HALFTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_HALFTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/parallel/device.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief A class which implement 1/2 as a type. Useful for squareroots.
   *
   * Unit test: make test-halftype
   **/
  struct HalfType {
    static std::string toString() { return "(HalfType)1/2"; }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION static constexpr double get(const IDX &...)
    {
      return 0.5;
    }

    /** \brief Need a static instance of halftype, for constructing `Power(T& a, HalfType& ht);` */
    static DEVICE_FORCEINLINE_FUNCTION HalfType &getStatic()
    {
      static HalfType ht;
      return ht;
    }

    static constexpr bool ISCONSTANT = true;

    // operator double() const { return 0.5; }
  };
} // namespace TempLat

#endif
