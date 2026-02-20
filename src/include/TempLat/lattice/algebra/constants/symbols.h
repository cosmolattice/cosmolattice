#ifndef TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_SYMBOLS_H
#define TEMPLAT_LATTICE_ALGEBRA_CONSTANTS_SYMBOLS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/operators/unaryminus.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{

  /** @brief A namespace which contains symbols such as the epsilon tensor.
   *
   *
   * Unit test: ctest -R test-symbols
   **/

  namespace Symbols
  {

    template <int I, int J> constexpr DEVICE_FORCEINLINE_FUNCTION auto delta(Tag<I>, Tag<J>) { return ZeroType(); }

    template <int I> constexpr DEVICE_FORCEINLINE_FUNCTION auto delta(Tag<I>, Tag<I>) { return OneType(); }

    template <int I, int J, int K>
    constexpr DEVICE_FORCEINLINE_FUNCTION auto epsilon(Tag<I>, Tag<J>, Tag<K>)
    {
      return ZeroType();
    }

    constexpr DEVICE_FORCEINLINE_FUNCTION auto epsilon(Tag<1>, Tag<2>, Tag<3>) { return OneType(); }

    DEVICE_FORCEINLINE_FUNCTION auto epsilon(Tag<1>, Tag<3>, Tag<2>) { return -OneType(); }

    constexpr DEVICE_FORCEINLINE_FUNCTION auto epsilon(Tag<3>, Tag<1>, Tag<2>) { return OneType(); }

    DEVICE_FORCEINLINE_FUNCTION auto epsilon(Tag<3>, Tag<2>, Tag<1>) { return -OneType(); }

    constexpr DEVICE_FORCEINLINE_FUNCTION auto epsilon(Tag<2>, Tag<3>, Tag<1>) { return OneType(); }

    DEVICE_FORCEINLINE_FUNCTION auto epsilon(Tag<2>, Tag<1>, Tag<3>) { return -OneType(); }

  }; // namespace Symbols

} // namespace TempLat

#endif
