#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_HASSU2DOUBLETGET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{

  /** @brief A concept which checks whether the object has a su2doubletget method or not.
   *
   * Unit test: ctest -R test-hassu2doubletget
   **/
  template <typename T>
  concept HasSU2DoubletGet = requires(T t, Tag<0> tag) { t.SU2DoubletGet(tag); };
} // namespace TempLat

#endif
