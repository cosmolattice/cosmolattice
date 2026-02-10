#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERSCALARVECTOR_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERSCALARVECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"

namespace TempLat
{
  /** @brief A concept which is part of the implementation of the scalar multiplication for the vector algebra.
   *
   * Unit test: make test-conditionalbinarygetterscalarvector
   **/
  template <typename S, typename T, bool OVERRIDE = true>
  concept ConditionalBinaryGetterScalarVector =
      ((HasVectorGetMethod<S> && !HasVectorGetMethod<T>) || (!HasVectorGetMethod<S> && HasVectorGetMethod<T>));

  struct ConditionalBinaryGetterScalarVectorTester {
    static void Test(TempLat::TDDAssertion &);
  };
} // namespace TempLat

#endif
