#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_HASCOMPLEXFIELDGET_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_HASCOMPLEXFIELDGET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{

  /** @brief A concept which checks if instance is a complex field.
   *
   * Unit test: ctest -R test-hascomplexfieldget
   **/
  template <class T>
  concept HasComplexFieldGet = requires(T t, Tag<0> tag) { t.ComplexFieldGet(tag); };
} // namespace TempLat

#endif
