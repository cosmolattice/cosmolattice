#ifndef TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERVECTOR_H
#define TEMPLAT_LATTICE_ALGEBRA_CONDITIONAL_CONDITIONALBINARYGETTERVECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"

namespace TempLat
{
  /** \brief A class which is part of the implementation of the list algebra. Conditionnaly returns some binary
   *operation type.
   *
   * Unit test: make test-conditionalbinarygetterscalarvector
   **/
  template <template <typename OPT, typename OPS> class OP, typename S, typename T>
  concept ConditionalListBinaryGetter =
      (IsSTDGettable<0, S> || IsSTDGettable<0, T> || IsTempLatGettable<0, S> || IsTempLatGettable<0, T>);

  struct ConditionalListBinaryGetterTester {
    static void Test(TempLat::TDDAssertion &);
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/conditional/conditionallistbinarygetter_test.h"
#endif

#endif
