#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructglobal.h"

namespace TempLat
{
  MakeException(LayoutStructLocalSizeException);

  /** @brief A class which
   *  localSizes: the local size in each dimension.
   *  localStart: the coordinate of the first entry in local memory, in the global coordinate space.
   *
   *
   * Unit test: ctest -R test-layoutstructlocal
   **/
  template <size_t _NDim> class LayoutStructLocal
  {
  public:
    static constexpr size_t NDim = _NDim;

    LayoutStructLocal(const device::IdxArray<NDim> &initNGrid, const device::Idx nGhosts)
        : mGlobal(initNGrid), mLocalStarts{}, mPadding{}, mNGhosts(nGhosts)
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalSizes[i] = initNGrid[i];
    }

    DEVICE_FORCEINLINE_FUNCTION
    LayoutStructGlobal<NDim> &getGlobal() { return mGlobal; }
    DEVICE_FORCEINLINE_FUNCTION
    const LayoutStructGlobal<NDim> &getGlobal() const { return mGlobal; }

    void setLocalSizes(const device::IdxArray<NDim> &input)
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalSizes[i] = input[i];
    }
    void setNGhosts(device::Idx nGhosts) { mNGhosts = nGhosts; }
    device::Idx getNGhosts() const { return mNGhosts; }

    void setPadding(const device::array<device::IdxArray<2>, NDim> &padding)
    {
      for (size_t i = 0; i < NDim; ++i) {
        mPadding[i][0] = padding[i][0];
        mPadding[i][1] = padding[i][1];
        if (mNGhosts != 0) {
          if (mPadding[i][0] != mNGhosts || mPadding[i][1] != mNGhosts)
            throw LayoutStructLocalSizeException("Padding and number of ghost cells must be the same.");
        }
      }
    }
    const device::array<device::IdxArray<2>, NDim> &getPadding() const { return mPadding; }

    DEVICE_FORCEINLINE_FUNCTION
    device::IdxArray<NDim> &getLocalSizes() { return mLocalSizes; }
    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getLocalSizes() const { return mLocalSizes; }

    void setLocalStarts(const device::IdxArray<NDim> &input)
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalStarts[i] = input[i];
    }
    DEVICE_FORCEINLINE_FUNCTION
    device::IdxArray<NDim> &getLocalStarts() { return mLocalStarts; }
    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getLocalStarts() const { return mLocalStarts; }

    /** @brief For both configuration and fourier space, the index values are not the same as coordinate
     *  values. Assuming periodic boundary conditions, we get that always c = i > half ? i - N : i;
     *  Don't mix up the arguments! Does not do transposition, so input pre-transposed dimension!
     */
    DEVICE_FORCEINLINE_FUNCTION
    device::Idx memoryIndexToSpatialCoordinate(device::Idx index, device::Idx dimension) const
    {
      return mGlobal.memoryIndexToSpatialCoordinate(index + mLocalStarts[dimension] - mNGhosts, dimension);
    }

    /** @brief Inverse of memoryIndexToSpatialCoordinate: get memory from position. */
    DEVICE_FORCEINLINE_FUNCTION
    device::Idx spatialCoordinateToMemoryIndex(device::Idx position, device::Idx dimension) const
    {
      return mGlobal.spatialCoordinateToMemoryIndex(position, dimension) - mLocalStarts[dimension] + mNGhosts;
    }

    template <size_t d2> friend bool operator==(const LayoutStructLocal<NDim> &a, const LayoutStructLocal<d2> &b)
    {
      if constexpr (NDim != d2)
        return false;
      else {
        bool result = a.mGlobal == b.mGlobal && a.mLocalSizes.size() == b.mLocalSizes.size() &&
                      a.mLocalStarts.size() == b.mLocalStarts.size();

        for (size_t i = 0; i < a.mLocalSizes.size(); ++i) {
          result = result && a.mLocalSizes[i] == b.mLocalSizes[i];
          result = result && a.mLocalStarts[i] == b.mLocalStarts[i];
        }
        return result;
      }
    }

    friend std::ostream &operator<<(std::ostream &ostream, const LayoutStructLocal &ls)
    {
      ostream << ls.mGlobal << "\n"
              << "  LocalSizes: " << ls.mLocalSizes << "\n"
              << "  LocalStarts: " << ls.mLocalStarts << "\n"
              << "  Padding: " << ls.mPadding << "\n";
      return ostream;
    }

  private:
    LayoutStructGlobal<NDim> mGlobal;
    device::IdxArray<NDim> mLocalSizes;
    device::IdxArray<NDim> mLocalStarts;
    device::array<device::IdxArray<2>, NDim> mPadding;
    device::Idx mNGhosts;
  };

} // namespace TempLat

#endif
