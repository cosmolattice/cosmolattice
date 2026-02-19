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
    using type = T;
  };

  template <typename S> struct GetFloatType<complex<S>> {
    using type = S;
  };
} // namespace TempLat

#endif
