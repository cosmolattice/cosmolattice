#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_SPATIALCOORDINATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/algebra/coordinates/coordinatevector.h"
#include "TempLat/lattice/algebra/helpers/getvectorcomponent.h"

namespace TempLat
{
  MakeException(SpatialCoordinateConfigWrongSpaceConfirmation);

  /** @brief A class which implements spatial coordinates.
   *
   * Unit test: ctest -R test-spatialcoordinate
   **/
  template <size_t NDim> class SpatialCoordinate : public CoordinateVector<NDim>
  {
  public:
    // Put public methods here. These should change very little over time.

    SpatialCoordinate(device::memory::host_ptr<MemoryToolBox<NDim>> toolBox)
        : mToolBox(toolBox), mLayout(toolBox->mLayouts.getConfigSpaceLayout())
    {
    }

    static constexpr ptrdiff_t getVectorSize() { return NDim; }

    template <typename IDX1, typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION auto vectorGet(const IDX1 component, const IDX &...idx) const
    {
      device::IdxArray<NDim> result;
      mLayout.putSpatialLocationFromMemoryIndexInto(result, idx...);
      return result[component];
    }

    virtual JumpsHolder<NDim> getJumps() const override { return mToolBox->mLayouts.getConfigSpaceJumps(); }

    virtual void confirmSpace(ptrdiff_t i, const LayoutStruct<NDim> &newLayout,
                              const SpaceStateType &spaceType) const override
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

    inline auto getToolBox() const
    { // just take toolbox from first component
      return mToolBox;
    }

    static std::string toString(ptrdiff_t j) { return "x_" + std::to_string(j); }
    static std::string toString() { return "x"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    device::memory::host_ptr<MemoryToolBox<NDim>> mToolBox;
    LayoutStruct<NDim> mLayout;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
