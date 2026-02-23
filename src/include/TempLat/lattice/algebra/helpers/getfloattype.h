#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETFLOATTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETFLOATTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A template which extracts the float type from complexes.
   *
   * Unit test: ctest -R test-getfloattype
   **/
  template <typename T> struct GetFloatType {
  };

  template <typename F>
    requires std::is_arithmetic_v<F>
  struct GetFloatType<F> {
    using type = F;
  };
  template <typename F> struct GetFloatType<complex<F>> {
    using type = F;
  };

  template <typename T, size_t N> struct GetFloatType<device::array<T, N>> {
    using type = typename GetFloatType<T>::type;
  };
} // namespace TempLat

#endif
