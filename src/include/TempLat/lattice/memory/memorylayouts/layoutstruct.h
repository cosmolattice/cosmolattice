#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocaltransposed.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/constexpr_for.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  MakeException(LayoutStructWrongSizeException);
  MakeException(LayoutStructOutOfBoundsExcetion);

  /** @brief Holds the result of FFTLibraryInterface::computeLocalSizes.
   *  In itself, has practically no logic, only forwards all the getters and setters to their respective owners.
   *  See LayoutStructGlobal, LayoutStructLocal and LayoutStructLocalTransposed.
   *
   *
   * Unit test: ctest -R test-layoutstruct
   **/
  template <size_t NDim> struct LayoutStruct {
    /**
     * @brief Create a new LayoutStruct object.
     *
     * @tparam C A container with NDim elements
     * @param initNGrid A container with NDim elements, each element is the size of the grid in that dimension.
     * @param nGhosts The number of ghost cells in each dimension.
     * @return requires
     */
    LayoutStruct(const device::IdxArray<NDim> &initNGrid, const device::Idx nGhosts)
        : mTransposed(initNGrid, nGhosts), mHermitianPartners(initNGrid)
    {
    }

    LayoutStruct() : mTransposed(device::IdxArray<NDim>{{1}}, 0), mHermitianPartners(device::IdxArray<NDim>{{1}}) {}

    /** @brief An almost constructor: return a new instance which has a default global FFT layout */
    static LayoutStruct<NDim> createGlobalFFTLayout(const device::IdxArray<NDim> &initNGrid)
    {
      LayoutStruct result(initNGrid, 0);
      result.getGlobal().getGlobalSizes()[NDim - 1] = result.getGlobal().getGlobalSizes()[NDim - 1] / 2 + 1;
      result.getLocal().getLocalSizes()[NDim - 1] = result.getGlobalSizes()[NDim - 1];
      return result;
    }

    template <typename T = double> DEVICE_FORCEINLINE_FUNCTION T getMaxRadius() const
    {
      return getGlobal().template getMaxRadius<T>();
    }

    DEVICE_FORCEINLINE_FUNCTION
    bool isTransposed() const { return getTransposed().isTransposed(); }

    /** @brief local index in some dimension of the memory layout, goes into its corresponding spatial dimension
     *  in the target memory. No bounds checking!
     */
    template <typename Container, typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION void putSpatialLocationFromMemoryIndexInto(Container &target, const IDX... idx) const
    {
      const auto indices = device::tie(idx...);
      constexpr_for<0, NDim>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        auto map = getTransposed().getSpatialLocationFromMemoryIndex(device::get<d>(indices), d);
        target[map.atIndex] = map.withValue;
      });
    }

    template <typename Container, typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION void putSpatialLocationFromMemoryIndexInto0N(Container &target, const IDX... idx)
        const // Brings back the coordinates between 0 and N-1. Useful for saving and loading for example
    {
      putSpatialLocationFromMemoryIndexInto(target, idx...);
      for (size_t j = 0; j < NDim; ++j)
        if (target[j] < 0) target[j] = target[j] + getGlobal().getGlobalSizes()[j];
    }

    /** @brief Inverse of putSpatialLocationFromMemoryIndexInto: from spatial
     *  coordinate to memory indices, in memory-layout order (that is,
     *  transposed.
     */
    template <typename Container, typename... IDX>
      requires IsVariadicNDIndex<NDim, IDX...>
    DEVICE_FORCEINLINE_FUNCTION bool putMemoryIndexFromSpatialLocationInto(Container &target, const IDX... pos) const
    {
      const auto positions = device::tie(pos...);
      bool owned = true;
      constexpr_for<0, NDim>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        auto map = getTransposed().getMemoryIndexFromSpatialLocation(device::get<d>(positions), d);
        target[map.atIndex] = map.withValue;
        owned &= map.owned;
      });
      return owned;
    }

    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getGlobalSizes() const { return getGlobal().getGlobalSizes(); }

    void setLocalSizes(const device::IdxArray<NDim> &input) { getTransposed().setLocalSizes(input); }

    void setNGhosts(const device::Idx &nGhosts) { getTransposed().setNGhosts(nGhosts); }

    void setPadding(const device::array<device::IdxArray<2>, NDim> &padding) { getLocal().setPadding(padding); }
    device::array<device::IdxArray<2>, NDim> getPadding() const { return getTransposed().getPadding(); }

    device::Idx getNGhosts() const { return getLocal().getNGhosts(); }

    device::IdxArray<NDim> &getLocalSizes() { return getLocal().getLocalSizes(); }
    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getLocalSizes() const { return getLocal().getLocalSizes(); }

    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getSizesInMemory() const { return getTransposed().getSizesInMemory(); }

    void setLocalStarts(const device::IdxArray<NDim> &input) { getLocal().setLocalStarts(input); }
    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getLocalStarts() const { return getLocal().getLocalStarts(); }

    void setTranspositionMap_memoryToGlobalSpace(const device::IdxArray<NDim> &input)
    {
      getTransposed().setTranspositionMap_memoryToGlobalSpace(input);
    }
    DEVICE_FORCEINLINE_FUNCTION
    const auto &getTranspositionMap_memoryToGlobalSpace() const
    {
      return getTransposed().getTranspositionMap_memoryToGlobalSpace();
    }

    device::Idx getOrigin() const { return getTransposed().getOrigin(); }

    device::Idx stride(size_t dim) const { return getTransposed().stride(dim); }

    void setHermitianPartners(HermitianPartners<NDim> &&newInstance) { mHermitianPartners = std::move(newInstance); }

    DEVICE_FORCEINLINE_FUNCTION
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

  private:
    LayoutStructLocalTransposed<NDim> mTransposed;
    /** @brief signed wavenumber and coordinate x = index > n/2 ? index - n : index. Need to provide this n/2 for each
     * dimensions. */
    HermitianPartners<NDim> mHermitianPartners;

    DEVICE_FORCEINLINE_FUNCTION
    LayoutStructLocalTransposed<NDim> &getTransposed() { return mTransposed; }
    DEVICE_FORCEINLINE_FUNCTION
    LayoutStructLocal<NDim> &getLocal() { return getTransposed().getLocal(); }
    DEVICE_FORCEINLINE_FUNCTION
    LayoutStructGlobal<NDim> &getGlobal() { return getLocal().getGlobal(); }

    DEVICE_FORCEINLINE_FUNCTION
    const LayoutStructLocalTransposed<NDim> &getTransposed() const { return mTransposed; }
    DEVICE_FORCEINLINE_FUNCTION
    const LayoutStructLocal<NDim> &getLocal() const { return getTransposed().getLocal(); }
    DEVICE_FORCEINLINE_FUNCTION
    const LayoutStructGlobal<NDim> &getGlobal() const { return getLocal().getGlobal(); }
  };

} // namespace TempLat

#endif
