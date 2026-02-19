#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat
{
  /** @brief A class which get real parts of fields.
   *
   *
   * Unit test: ctest -R test-real
   **/
  template <class T>
    requires requires(T t) { t.ComplexFieldGet(0_c); }
  static DEVICE_FORCEINLINE_FUNCTION auto Real(T &&t)
  {
    return t.ComplexFieldGet(0_c);
  }

  template <class T>
    requires IsComplexType<T>
  static DEVICE_FORCEINLINE_FUNCTION auto Real(T &&t)
  {
    return t.real();
  }
} // namespace TempLat

#endif
