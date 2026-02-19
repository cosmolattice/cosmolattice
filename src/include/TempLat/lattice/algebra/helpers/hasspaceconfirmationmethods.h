#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASSPACECONFIRMATIONMETHODS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/util/rangeiteration/tag.h"

namespace TempLat
{
  template <class T, size_t NDim>
  concept HasSpaceConfirmationMethods = requires(std::decay_t<T> t, LayoutStruct<NDim> layout,
                                                 SpaceStateType spaceType) { t.confirmSpace(layout, spaceType); };

  template <int N, class T, size_t NDim>
  concept HasSpaceConfirmationMethodsIndexed =
      requires(Tag<N> tag, std::decay_t<T> t, LayoutStruct<NDim> layout, SpaceStateType spaceType) {
        t.confirmSpace(tag, layout, spaceType);
      };

  template <class T, size_t NDim>
  concept HasSpaceConfirmationMethodsDirectIndexed =
      requires(ptrdiff_t idx, std::decay_t<T> t, LayoutStruct<NDim> layout, SpaceStateType spaceType) {
        t.confirmSpace(idx, layout, spaceType);
      };

  template <int N, class T, size_t NDim>
  concept HasSpaceConfirmationMethodsElement =
      requires(Tag<N> tag, std::decay_t<T> t, LayoutStruct<NDim> layout, SpaceStateType spaceType) {
        t(tag).confirmSpace(layout, spaceType);
      };

  template <class T, size_t NDim>
  concept HasSpaceConfirmationMethodsDirectElement =
      requires(ptrdiff_t idx, std::decay_t<T> t, LayoutStruct<NDim> layout, SpaceStateType spaceType) {
        t(idx).confirmSpace(layout, spaceType);
      };
} // namespace TempLat

#endif
