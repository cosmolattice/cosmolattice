#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALUNARYGETTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/haseval.h"

namespace TempLat
{
  /** @brief A concept which is used to enably our operator overloads only when the input type
   *  has a get method. This avoids overriding the unary operators
   *  everywhere else. We don't want to override operator-(double), for example.
   *
   * Unit test: ctest -R test-conditionalunarygetter
   **/
  template <typename T>
  concept ConditionalUnaryGetter = HasEvalMethod<T>;
} // namespace TempLat

#endif
