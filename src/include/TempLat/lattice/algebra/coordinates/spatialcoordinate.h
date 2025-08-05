#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/coordinates/coordinatevector.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"

namespace TempLat
{
  /** \brief A class which implements spatial coordinates.
   *
   * Unit test: make test-spatialcoordinate
   **/
  MakeException(SpatialCoordinateConfigWrongSpaceConfirmation);

  template <size_t NDim> class SpatialCoordinate : public CoordinateVector<NDim>
  {
    Kokkos::Array<ptrdiff_t, NDim> mPadding;
    LayoutStruct<NDim> mLayout;

  public:
    /* Put public methods here. These should change very little over time. */
    SpatialCoordinate(std::shared_ptr<MemoryToolBox<NDim>> toolBox)
        : mToolBox(toolBox), mLayout(toolBox->mLayouts.getConfigSpaceLayout())
    {
      auto configSpaceJumps = mToolBox->mLayouts.getConfigSpaceJumps();
      for (size_t d = 0; d < NDim; ++d)
        mPadding[d] = configSpaceJumps.getPadding()[d][0];
    }

    static constexpr ptrdiff_t getVectorSize() { return NDim; }

    // usage: f(input1, input2, input3, output);

    template <std::integral... IDX>
      requires(sizeof...(IDX) == NDim + 1)
    KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      return get_impl(std::tie(idx...), std::make_index_sequence<sizeof...(IDX) - 1>{});
    }

    template <std::integral... IDX, size_t... InputIndexes>
    KOKKOS_FORCEINLINE_FUNCTION auto get_impl(std::tuple<const IDX &...> allIdx,
                                              std::index_sequence<InputIndexes...>) const
    {
      auto constexpr lastIdx = sizeof...(IDX) - 1;
      return get_impl(std::get<lastIdx>(allIdx), std::get<InputIndexes>(allIdx)...);
    }

    template <std::integral IDX1, std::integral... IDX>
    KOKKOS_FORCEINLINE_FUNCTION auto get_impl(const IDX1 component, const IDX &...idx) const
    {
      using type = decltype(component * (idx * ...));
      static_assert(std::is_integral_v<type>, "SpatialCoordinate only supports integral types.");
      Kokkos::Array<ptrdiff_t, NDim> result;
      mLayout.putSpatialLocationFromMemoryIndexInto(result, idx...);
      return result[component];
    }

    JumpsHolder<NDim> getJumps() { return mToolBox->mLayouts.getConfigSpaceJumps(); }

    void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout,
                      const SpaceStateInterface<NDim>::SpaceType &spaceType)
    {
      switch (spaceType) {
      case SpaceStateInterface<NDim>::SpaceType::Fourier:
        throw SpatialCoordinateConfigWrongSpaceConfirmation(
            "SpatialCoordinate explicitly only can be used in configuration space. Abort.");
        break;
      case SpaceStateInterface<NDim>::SpaceType::Configuration:
      default:
        break;
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::shared_ptr<MemoryToolBox<NDim>> mToolBox;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
