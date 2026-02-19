#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"

namespace TempLat
{
  MakeException(SpatialCoordinateConfigWrongSpaceConfirmation);

  /** @brief A class which implements spatial coordinates.
   *
   * Unit test: ctest -R test-spatialcoordinate
   **/
  template <size_t NDim> class SpatialCoordinate
  {
  public:
    // Put public methods here. These should change very little over time.

    SpatialCoordinate(device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : mToolBox(toolBox), mLayout(toolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    static constexpr ptrdiff_t getVectorSize() { return NDim; }

    template <typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::IdxArray<NDim> result;
      mLayout.putSpatialLocationFromMemoryIndexInto(result, idx...);
      return result;
    }

    void doWeNeedGhosts(ptrdiff_t i) const {}
    template <int N> ptrdiff_t confirmGhostsUpToDate(Tag<N> i) const { return 1; }

    virtual void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      switch (spaceType) {
      case SpaceStateType::Fourier:
        throw SpatialCoordinateConfigWrongSpaceConfirmation(
            "SpatialCoordinate explicitly only can be used in configuration space. Abort.");
        break;
      case SpaceStateType::Configuration:
      default:
        break;
      }
    }

    template <int N>
      requires(N > 0)
    auto vectorGet(Tag<N> t) const
    {
      return getVectorComponent(*this, Tag<N - 1>());
    }

    template <int N>
      requires(N > 0)
    auto operator()(Tag<N> t) const
    {
      return vectorGet(t);
    }

    inline auto getToolBox() const { return mToolBox; }

    static std::string toString(ptrdiff_t j) { return "x_" + std::to_string(j); }
    static std::string toString() { return "x"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;
    LayoutStruct<NDim> mLayout;
  };
} // namespace TempLat

#endif
