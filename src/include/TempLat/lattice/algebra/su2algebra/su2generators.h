#ifndef COSMOINTERFACE_SU2ALGEBRA_PAULIMATRICES_H
#define COSMOINTERFACE_SU2ALGEBRA_PAULIMATRICES_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which implements the i*sigma matrices
   *
   * Unit test: ctest -R test-paulimatrices
   **/
  namespace Constants
  {
    struct Sigma0 {
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr OneType SU2Get(Tag<0> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<1> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<2> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<3> t) { return {}; }

      static std::string toString() { return "Id"; }
      static constexpr size_t size = 4;
    };
    constexpr Sigma0 sigma0;

    struct Sigma1 {
      static constexpr ZeroType SU2Get(Tag<0> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr OneType SU2Get(Tag<1> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<2> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<3> t) { return {}; }

      static std::string toString() { return "i\u03C3_1"; }
      static constexpr size_t size = 4;
    };
    constexpr Sigma1 sigma1;

    struct Sigma2 {
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<0> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<1> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr OneType SU2Get(Tag<2> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<3> t) { return {}; }

      static std::string toString() { return "i\u03C3_2"; }
      static constexpr size_t size = 4;
    };
    constexpr Sigma2 sigma2;

    struct Sigma3 {
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<0> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<1> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr ZeroType SU2Get(Tag<2> t) { return {}; }
      DEVICE_FORCEINLINE_FUNCTION
      static constexpr OneType SU2Get(Tag<3> t) { return {}; }

      static std::string toString() { return "i\u03C3_3"; }
      static constexpr size_t size = 4;
    };
    constexpr Sigma3 sigma3;

    DEVICE_FORCEINLINE_FUNCTION
    constexpr Sigma0 i_sigma(Tag<0> t) { return sigma0; }
    DEVICE_FORCEINLINE_FUNCTION
    constexpr Sigma1 i_sigma(Tag<1> t) { return sigma1; }
    DEVICE_FORCEINLINE_FUNCTION
    constexpr Sigma2 i_sigma(Tag<2> t) { return sigma2; }
    DEVICE_FORCEINLINE_FUNCTION
    constexpr Sigma3 i_sigma(Tag<3> t) { return sigma3; }
  }; // namespace Constants
} // namespace TempLat

#endif
