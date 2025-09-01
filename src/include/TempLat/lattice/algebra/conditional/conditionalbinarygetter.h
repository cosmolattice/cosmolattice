#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  template <typename S, typename T>
  concept ConditionalBinaryGetter = requires {
    requires HasGetMethod<S> || HasGetMethod<T>;
    requires !HasComplexFieldGet<S> && !HasComplexFieldGet<T>;
    requires !HasSU2Get<S> && !HasSU2Get<T>;
    requires !HasVectorGetMethod<S>;
    requires !HasVectorGetMethod<T> && !HasStaticGetter<S> && !HasStaticGetter<T>;
  };

  struct ConditionalBinaryGetterTester {
    static void Test(TempLat::TDDAssertion &);
  };
} // namespace TempLat

#endif
