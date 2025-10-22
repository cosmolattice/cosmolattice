#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocal.h"
#include "TempLat/lattice/memory/memorylayouts/transpositionmap.h"

namespace TempLat
{

  MakeException(LayoutStructLocalTransposedSizeException);

  /** \brief
   *  transitionMap: a map of the dimensions, for transposition in memory. E.g. {0, 1, 2, 3} is untransposed 4d, {1, 0,
   *2, 3} is FFTW transposed. In other words, the entries in transitionMap are in line with the entries in
   *localStarts/-Sizes, and their value indicates the physical-space dimension of that entry.
   *
   * Also, this class manages the sizes in memory!
   *
   * Unit test: make test-layoutstructlocaltransposed
   **/
  template <size_t _NDim> class LayoutStructLocalTransposed
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t NDim = _NDim;

    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsNDArray<C, NDim>
    LayoutStructLocalTransposed(const C &initNGrid, const ptrdiff_t nGhosts)
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

    template <typename C = device::array<ptrdiff_t, NDim>> void setLocalSizes(const C &input)
    {
      getLocal().setLocalSizes(input);
      adaptMemorySizesFromTranspositionMap();
    }
    void setNGhosts(ptrdiff_t nGhosts)
    {
      mNGhosts = nGhosts;
      getLocal().setNGhosts(nGhosts);
    }

    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsNDArray<C, NDim>
    void setTranspositionMap_memoryToGlobalSpace(const C &input)
    {
      mTranspositionMap_memoryToGlobalSpace.setMap(input);
      adaptMemorySizesFromTranspositionMap();
    }
    DEVICE_FORCEINLINE_FUNCTION
    const TranspositionMap<NDim> &getTranspositionMap_memoryToGlobalSpace() const
    {
      return mTranspositionMap_memoryToGlobalSpace;
    }

    DEVICE_FORCEINLINE_FUNCTION
    const device::array<ptrdiff_t, NDim> &getSizesInMemory() const { return mSizesInMemory; }

    /** \brief A dictionary for return values for memory to coordinate mapping. */
    struct CoordinateMapping {
      ptrdiff_t atIndex, withValue;
    };

    /** \brief With transposition, go from actual memory index in memoryDimension to spatial coordinate value at spatial
     * dimension. */
    DEVICE_FORCEINLINE_FUNCTION
    CoordinateMapping getSpatialLocationFromMemoryIndex(ptrdiff_t index, ptrdiff_t memoryDimension) const
    {
      CoordinateMapping result;
      result.atIndex = mTranspositionMap_memoryToGlobalSpace.getForward(memoryDimension);
      /* in this function we pass the new forward transposed index to the mLocal. */
      result.withValue = mLocal.memoryIndexToSpatialCoordinate(index, result.atIndex);
      return result;
    }

    /** \brief With transposition, go from spatial coordinate value at spatial dimension to actual memory index in
     * memoryDimension. */
    DEVICE_FORCEINLINE_FUNCTION
    CoordinateMapping getMemoryIndexFromSpatialLocation(ptrdiff_t position, ptrdiff_t spatialDimension) const
    {
      CoordinateMapping result;
      result.atIndex = mTranspositionMap_memoryToGlobalSpace.getInverse(spatialDimension);
      /* in this function we pass the input untransposed index to the mLocal. */
      result.withValue = mLocal.spatialCoordinateToMemoryIndex(position, spatialDimension);
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
    device::array<ptrdiff_t, NDim> mSizesInMemory;
    ptrdiff_t mNGhosts;

    void adaptMemorySizesFromTranspositionMap()
    {
      for (ptrdiff_t i = 0, iEnd = std::min(mSizesInMemory.size(), mTranspositionMap_memoryToGlobalSpace.size());
           i < iEnd; ++i) {
        ptrdiff_t getSizeIndex = mTranspositionMap_memoryToGlobalSpace.getForward(i);
        mSizesInMemory[i] = mLocal.getLocalSizes()[getSizeIndex];
      }
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
