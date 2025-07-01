#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <memory>
#include <numeric>

#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocaltransposed.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  MakeException(LayoutStructWrongSizeException);
  MakeException(LayoutStructOutOfBoundsException);

  /** \brief Holds the result of FFTLibraryInterface::computeLocalSizes.
   *  In itself, has practically no logic, only forwards all the getters and setters to their respective owners.
   *  See LayoutStructGlobal, LayoutStructLocal and LayoutStructLocalTransposed.
   *
   *
   * Unit test: make test-layoutstruct
   **/

  /** \brief The result holder for computeLocalSizes. It is your responsibility to set all values. */
  template <size_t NDim> struct LayoutStruct {
    LayoutStruct(std::array<ptrdiff_t, NDim> initNGrid) : mTransposed(initNGrid), mHermitianPartners(initNGrid) {}

    /** \brief An almost constructor: return a new instance which has a default global FFT layout */
    static LayoutStruct<NDim> createGlobalFFTLayout(std::array<ptrdiff_t, NDim> initNGrid)
    {
      LayoutStruct result(initNGrid);
      result.getGlobal().getGlobalSizes()[NDim - 1] = result.getGlobal().getGlobalSizes()[NDim - 1] / 2 + 1;
      result.getLocal().getLocalSizes()[NDim - 1] = result.getGlobalSizes()[NDim - 1];
      return result;
    }

    template <typename T = double> T getMaxRadius() const { return getGlobal().template getMaxRadius<T>(); }

    bool isTransposed() const { return getTransposed().isTransposed(); }

    /** \brief local index in some dimension of the memory layout, goes into its corresponding spatial dimension
     *  in the target memory. No bounds checking!
     */
    void putSpatialLocationFromMemoryIndexInto(ptrdiff_t index, ptrdiff_t memoryDimension,
                                               std::array<ptrdiff_t, NDim> &target) const
    {
      auto map = getTransposed().getSpatialLocationFromMemoryIndex(index, memoryDimension);
      target[map.atIndex] = map.withValue;
    }

    /** \brief Inverse of putSpatialLocationFromMemoryIndexInto: from spatial
     *  coordinate to memory indices, in memory-layout order (that is,
     *  transposed, ready to be applied to `JumpsHolder::getJumpsInMemoryOrder()`.
     */
    void putMemoryIndexFromSpatialLocationInto(ptrdiff_t position, ptrdiff_t spatialDimension,
                                               std::array<ptrdiff_t, NDim> &target) const
    {
      auto map = getTransposed().getMemoryIndexFromSpatialLocation(position, spatialDimension);
      target[map.atIndex] = map.withValue;
    }

    const std::array<ptrdiff_t, NDim> &getGlobalSizes() const { return getGlobal().getGlobalSizes(); }

    template <typename T = ptrdiff_t> void setLocalSizes(const std::array<T, NDim> &input)
    {
      getTransposed().setLocalSizes(input);
    }

    std::array<ptrdiff_t, NDim> &getLocalSizes() { return getLocal().getLocalSizes(); }
    const std::array<ptrdiff_t, NDim> &getLocalSizes() const { return getLocal().getLocalSizes(); }

    const std::array<ptrdiff_t, NDim> &getSizesInMemory() const { return getTransposed().getSizesInMemory(); }

    template <typename T = ptrdiff_t> void setLocalStarts(const std::array<T, NDim> &input)
    {
      getLocal().setLocalStarts(input);
    }
    std::array<ptrdiff_t, NDim> &getLocalStarts() { return getLocal().getLocalStarts(); }
    const std::array<ptrdiff_t, NDim> &getLocalStarts() const { return getLocal().getLocalStarts(); }

    template <typename T = ptrdiff_t> void setTranspositionMap_memoryToGlobalSpace(const std::array<T, NDim> &input)
    {
      getTransposed().setTranspositionMap_memoryToGlobalSpace(input);
    }
    const auto &getTranspositionMap_memoryToGlobalSpace() const
    {
      return getTransposed().getTranspositionMap_memoryToGlobalSpace();
    }

    void setHermitianPartners(HermitianPartners<NDim> &&newInstance) { mHermitianPartners = std::move(newInstance); }

    KOKKOS_FORCEINLINE_FUNCTION
    const auto &getHermitianPartners() const { return mHermitianPartners; }

    template <size_t d2> friend bool operator==(const LayoutStruct<NDim> &a, const LayoutStruct<d2> &b)
    {
      if constexpr (NDim != d2)
        return false;
      else {
        bool result = a.mTransposed == b.mTransposed && a.mHermitianPartners == b.mHermitianPartners;
        return result;
      }
    }

    friend std::ostream &operator<<(std::ostream &ostream, const LayoutStruct &ls)
    {
      ostream << ls.mTransposed << "\n"
              << "  Hermitian layout: " << ls.mHermitianPartners << "\n";
      return ostream;
    }

    //        friend class FFTLayoutStruct;
    //        friend class TripleStateLayouts;

  private:
    LayoutStructLocalTransposed<NDim> mTransposed;
    /** \brief signed wavenumber and coordinate x = index > n/2 ? index - n : index. Need to provide this n/2 for each
     * dimensions. */
    HermitianPartners<NDim> mHermitianPartners;

    inline LayoutStructLocalTransposed<NDim> &getTransposed() { return mTransposed; }
    inline LayoutStructLocal<NDim> &getLocal() { return getTransposed().getLocal(); }
    inline LayoutStructGlobal<NDim> &getGlobal() { return getLocal().getGlobal(); }

    inline const LayoutStructLocalTransposed<NDim> &getTransposed() const { return mTransposed; }
    inline const LayoutStructLocal<NDim> &getLocal() const { return getTransposed().getLocal(); }
    inline const LayoutStructGlobal<NDim> &getGlobal() const { return getLocal().getGlobal(); }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
