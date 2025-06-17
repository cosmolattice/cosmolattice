#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/void_t.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  template <class T, size_t NDim>
  concept HasSpaceConfirmationMethods =
      requires(T t, LayoutStruct<NDim> layout, SpaceStateInterface<NDim>::SpaceType spaceType) {
        t.confirmSpace(layout, spaceType);
      };

  template <int N, class T, size_t NDim>
  concept HasSpaceConfirmationMethodsIndexed =
      requires(Tag<N> tag, T t, LayoutStruct<NDim> layout, SpaceStateInterface<NDim>::SpaceType spaceType) {
        t.confirmSpace(tag, layout, spaceType);
      };

  template <class T, size_t NDim>
  concept HasSpaceConfirmationMethodsIndexedDyn =
      requires(ptrdiff_t idx, T t, LayoutStruct<NDim> layout, SpaceStateInterface<NDim>::SpaceType spaceType) {
        t.confirmSpace(idx, layout, spaceType);
      };

  /** \brief a mini tester class... */
  struct HasSpaceConfirmationMethodsTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasspaceconfirmationmethods_test.h"
#endif

#endif
