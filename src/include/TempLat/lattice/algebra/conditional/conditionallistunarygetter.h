#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALLISTUNARYGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALLISTUNARYGETTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"

namespace TempLat
{
  /** @brief A class which is part of the implementation of the list algebra.
   *
   * Unit test: ctest -R test-conditionallistunarygetter
   **/
  template <typename S>
  concept ConditionalListUnaryGetter = IsSTDGettable<0, S> || IsTempLatGettable<0, S>;
} // namespace TempLat

#endif
