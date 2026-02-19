#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_DIMENSIONCOUNTRECORDER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/spacestateinterface.h"

namespace TempLat
{
  MakeException(DimensionCountRecorderException);
  MakeException(DimensionCountRecorder_CoordinateSpaceException);

  /** @brief A super class which records the number of dimensions on any
   *  call to confirmSpace.
   *
   *  The constructor takes one argument, which is used by the derived classes which insist on a single space only:
   *WaveNumber and SpatialCoordinate.
   *
   * Unit test: ctest -R test-dimensioncountrecorder
   **/
  template <size_t NDim> class DimensionCountRecorder
  {
  public:
    // Put public methods here. These should change very little over time.
    DimensionCountRecorder(SpaceStateType spaceType)
        : mFixedSingleSpaceType(spaceType), mCurrentSpaceType(spaceType), mCurrentLayout({}, 0)
    {
    }

    /** @brief When making sure everyone is in configuration or fourier space, steal the number of dimensions, which we
     * need in the coordinate manipulating objects.. */
    DEVICE_FUNCTION
    void confirmSpace(const LayoutStruct<NDim> &newLayout, const SpaceStateType &spaceType) const
    {
      if (mFixedSingleSpaceType != SpaceStateType::undefined && mFixedSingleSpaceType != spaceType) {
#ifndef DEVICE_KOKKOS
        throw DimensionCountRecorder_CoordinateSpaceException(
            "You are using coordinates in one space for an expression in another space. This coordinate object insists "
            "on",
            SpaceStateInterface<NDim>::SpaceTypeString(mFixedSingleSpaceType), "while you now ask for",
            SpaceStateInterface<NDim>::SpaceTypeString(spaceType));
#endif
      }

      mCurrentSpaceType = spaceType;
      mCurrentLayout = newLayout;
    }

    DEVICE_FORCEINLINE_FUNCTION
    static constexpr ptrdiff_t getNDimensions() { return NDim; }

    DEVICE_FORCEINLINE_FUNCTION
    SpaceStateType getCurrentSpaceType() const { return mCurrentSpaceType; }

    DEVICE_FORCEINLINE_FUNCTION
    const LayoutStruct<NDim> &getCurrentLayout() const { return mCurrentLayout; }

    std::string toString() const
    {
      return SpaceStateInterface<NDim>::SpaceTypetoCanonicalCharacter(mFixedSingleSpaceType);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    mutable SpaceStateType mFixedSingleSpaceType;
    mutable SpaceStateType mCurrentSpaceType;
    mutable LayoutStruct<NDim> mCurrentLayout;
  };
} // namespace TempLat

#endif
