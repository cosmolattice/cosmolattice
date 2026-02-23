#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMSPACE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMSPACE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/helpers/hasspaceconfirmationmethods.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"

namespace TempLat
{
  /** @brief A class which wraps the calls to confirm(Configuration/Fourier)Space, for classes that have it and objects
   *that don't.
   *
   * Unit test: ctest -R test-confirmspace
   **/
  class ConfirmSpace
  {
  public:
    // Put public methods here. These should change very little over time.
    template <typename U, size_t NDim>
      requires HasSpaceConfirmationMethods<U, NDim>
    static inline void apply(U &obj, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      obj.confirmSpace(newLayout, spaceType);
    }

    template <typename U, size_t NDim>
      requires(!HasSpaceConfirmationMethods<U, NDim>)
    static constexpr inline void apply(U &obj, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
    }

    template <int N, typename U, size_t NDim>
      requires HasSpaceConfirmationMethodsIndexed<N, U, NDim>
    static inline void apply(U &&obj, Tag<N> i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      obj.confirmSpace(i, newLayout, spaceType);
    }

    template <int N, typename U, size_t NDim>
      requires(!HasSpaceConfirmationMethodsIndexed<N, U, NDim> && HasSpaceConfirmationMethodsElement<N, U, NDim>)
    static inline void apply(U &&obj, Tag<N> i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      obj(i).confirmSpace(newLayout, spaceType);
    }

    template <int N, typename U, size_t NDim>
      requires(!HasSpaceConfirmationMethodsIndexed<N, U, NDim> && !HasSpaceConfirmationMethodsElement<N, U, NDim>)
    static inline void apply(U &obj, Tag<N> i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
    }

    template <typename U, size_t NDim>
      requires HasSpaceConfirmationMethodsDirectIndexed<U, NDim>
    static inline void apply(U &obj, ptrdiff_t i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      obj.confirmSpace(i, newLayout, spaceType);
    }

    template <typename U, size_t NDim>
      requires(!HasSpaceConfirmationMethodsDirectIndexed<U, NDim> && HasSpaceConfirmationMethodsDirectElement<U, NDim>)
    static inline void apply(U &obj, ptrdiff_t i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
      obj(i).confirmSpace(newLayout, spaceType);
    }

    template <typename U, size_t NDim>
      requires(!HasSpaceConfirmationMethodsDirectIndexed<U, NDim> && !HasSpaceConfirmationMethodsDirectElement<U, NDim>)
    static inline void apply(U &obj, ptrdiff_t i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType)
    {
    }

    ConfirmSpace() = delete;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };
} // namespace TempLat

#endif
