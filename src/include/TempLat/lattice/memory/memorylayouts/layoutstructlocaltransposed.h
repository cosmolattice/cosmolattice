#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocal.h"
#include "TempLat/lattice/memory/memorylayouts/transpositionmap.h"

namespace TempLat
{

  MakeException(LayoutStructLocalTransposedSizeException);

  /** @brief
   *  transitionMap: a map of the dimensions, for transposition in memory. E.g. {0, 1, 2, 3} is untransposed 4d, {1, 0,
   * 2, 3} is FFTW transposed. In other words, the entries in transitionMap are in line with the entries in
   * localStarts/-Sizes, and their value indicates the physical-space dimension of that entry.
   *
   * Also, this class manages the sizes in memory!
   *
   * Unit test: ctest -R test-layoutstructlocaltransposed
   **/
  template <size_t _NDim> class LayoutStructLocalTransposed
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t NDim = _NDim;

    LayoutStructLocalTransposed(const device::IdxArray<NDim> &initNGrid, const device::Idx nGhosts)
        : mLocal(initNGrid, nGhosts), mNGhosts(nGhosts)
    {
      for (size_t i = 0; i < NDim; ++i)
        mSizesInMemory[i] = mLocal.getLocalSizes()[i];
    }

    DEVICE_FORCEINLINE_FUNCTION
    LayoutStructLocal<NDim> &getLocal() { return mLocal; }
    DEVICE_FORCEINLINE_FUNCTION
    const LayoutStructLocal<NDim> &getLocal() const { return mLocal; }
    DEVICE_FORCEINLINE_FUNCTION
    LayoutStructGlobal<NDim> &getGlobal() { return getLocal().getGlobal(); }
    DEVICE_FORCEINLINE_FUNCTION
    const LayoutStructGlobal<NDim> &getGlobal() const { return getLocal().getGlobal(); }

    DEVICE_FORCEINLINE_FUNCTION
    bool isTransposed() const { return mTranspositionMap_memoryToGlobalSpace.isTransposed(); }

    void setLocalSizes(const device::IdxArray<NDim> &input)
    {
      getLocal().setLocalSizes(input);
      adaptMemorySizesFromTranspositionMap();
    }
    void setNGhosts(device::Idx nGhosts)
    {
      mNGhosts = nGhosts;
      getLocal().setNGhosts(nGhosts);
    }

    void setTranspositionMap_memoryToGlobalSpace(const device::IdxArray<NDim> &input)
    {
      mTranspositionMap_memoryToGlobalSpace.setMap(input);
      adaptMemorySizesFromTranspositionMap();
    }
    DEVICE_FORCEINLINE_FUNCTION
    const TranspositionMap<NDim> &getTranspositionMap_memoryToGlobalSpace() const
    {
      return mTranspositionMap_memoryToGlobalSpace;
    }

    device::array<device::IdxArray<2>, NDim> getPadding() const
    {
      const auto &result = getLocal().getPadding();
      // Needs transposition.
      device::array<device::IdxArray<2>, NDim> transposedResult;
      for (size_t i = 0; i < NDim; ++i) {
        device::Idx getPaddingIndex = mTranspositionMap_memoryToGlobalSpace.getForward(i);
        transposedResult[i][0] = result[getPaddingIndex][0];
        transposedResult[i][1] = result[getPaddingIndex][1];
      }
      return transposedResult;
    }

    /**
     * @brief Return the sizes in memory, i.e. with transposition applied. This does NOT include padding, but only fixes
     * the transposition.
     *
     * @return const ref to device::IdxArray<NDim> with the sizes.
     */
    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getSizesInMemory() const { return mSizesInMemory; }

    /**
     * @brief Get the actual memory index of the origin (WITH PADDING)
     *
     * @return device::Idx giving the (linear) memory index of the origin.
     */
    device::Idx getOrigin() const
    {
      const auto &sizes = getSizesInMemory();
      const auto &padding = getPadding();
      device::Idx origin = 0;
      device::Idx stride = 1;
      for (device::Idx i = NDim - 1; i >= 0; --i) {
        origin += padding[i][0] * stride;
        stride *= sizes[i] + padding[i][0] + padding[i][1];
      }
      return origin;
    }

    /**
     * @brief Mostly needed for debugging purposes, gets the stride in memory for a shift of 1 in the given
     * dimension, with transposition taken into account. The difference to the memorySizes or localSizes is that this
     * takes the padding into account.
     *
     * @param dim the dimension for which to compute the stride.
     * @return device::Idx stride in memory for the given dimension.
     */
    device::Idx stride(size_t dim) const
    {
      const auto &sizes = getSizesInMemory();
      const auto &padding = getPadding();
      device::Idx result = 1;
      for (size_t i = dim + 1; i < NDim; ++i) {
        result *= sizes[i] + padding[i][0] + padding[i][1];
      }
      return result;
    }

    /** @brief A dictionary for return values for memory to coordinate mapping. */
    struct CoordinateMapping {
      device::Idx atIndex, withValue;
      bool owned;
    };

    /** @brief With transposition, go from actual memory index in memoryDimension to spatial coordinate value at spatial
     * dimension. */
    DEVICE_FORCEINLINE_FUNCTION
    CoordinateMapping getSpatialLocationFromMemoryIndex(device::Idx index, device::Idx memoryDimension) const
    {
      CoordinateMapping result;
      result.atIndex = mTranspositionMap_memoryToGlobalSpace.getForward(memoryDimension);
      /* in this function we pass the new forward transposed index to the mLocal. */
      result.withValue = mLocal.memoryIndexToSpatialCoordinate(index, result.atIndex);
      result.owned = index >= 0 && index < mLocal.getLocalSizes()[memoryDimension];

      return result;
    }

    /** @brief With transposition, go from spatial coordinate value at spatial dimension to actual memory index in
     * memoryDimension. */
    DEVICE_FORCEINLINE_FUNCTION
    CoordinateMapping getMemoryIndexFromSpatialLocation(device::Idx position, device::Idx spatialDimension) const
    {
      CoordinateMapping result;
      result.atIndex = mTranspositionMap_memoryToGlobalSpace.getInverse(spatialDimension);
      /* in this function we pass the input untransposed index to the mLocal. */
      result.withValue = mLocal.spatialCoordinateToMemoryIndex(position, spatialDimension);
      result.owned = result.withValue >= 0 && result.withValue < mLocal.getLocalSizes()[result.atIndex];
      return result;
    }

    template <size_t d2>
    friend bool operator==(const LayoutStructLocalTransposed<NDim> &a, const LayoutStructLocalTransposed<d2> &b)
    {
      if constexpr (NDim != d2)
        return false;
      else {
        bool result = a.mLocal == b.mLocal &&
                      a.mTranspositionMap_memoryToGlobalSpace == b.mTranspositionMap_memoryToGlobalSpace &&
                      a.mSizesInMemory.size() == b.mSizesInMemory.size();

        for (size_t i = 0; i < a.mSizesInMemory.size(); ++i) {
          result = result && a.mSizesInMemory[i] == b.mSizesInMemory[i];
        }
        return result;
      }
    }

    friend std::ostream &operator<<(std::ostream &ostream, const LayoutStructLocalTransposed &ls)
    {
      ostream << ls.mLocal << "\n"
              << "  SizesInMemory: " << ls.mSizesInMemory << "\n"
              << "  TranspositionMap_memoryToGlobalSpace: " << ls.mTranspositionMap_memoryToGlobalSpace << "\n";
      return ostream;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    LayoutStructLocal<NDim> mLocal;
    TranspositionMap<NDim> mTranspositionMap_memoryToGlobalSpace;
    device::IdxArray<NDim> mSizesInMemory;
    device::Idx mNGhosts;

    void adaptMemorySizesFromTranspositionMap()
    {
      for (device::Idx i = 0, iEnd = std::min(mSizesInMemory.size(), mTranspositionMap_memoryToGlobalSpace.size());
           i < iEnd; ++i) {
        device::Idx getSizeIndex = mTranspositionMap_memoryToGlobalSpace.getForward(i);
        mSizesInMemory[i] = mLocal.getLocalSizes()[getSizeIndex];
      }
    }
  };

} // namespace TempLat

#endif
