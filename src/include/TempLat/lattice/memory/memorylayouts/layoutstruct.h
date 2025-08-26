#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocaltransposed.h"
#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/isarray.h"
#include "TempLat/util/constexpr_for.h"

namespace TempLat
{
  MakeException(LayoutStructWrongSizeException);
  MakeException(LayoutStructOutOfBoundsExcetion);

  /** \brief Holds the result of FFTLibraryInterface::computeLocalSizes.
   *  In itself, has practically no logic, only forwards all the getters and setters to their respective owners.
   *  See LayoutStructGlobal, LayoutStructLocal and LayoutStructLocalTransposed.
   *
   *
   * Unit test: make test-layoutstruct
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
    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsArray<C, NDim>
    LayoutStruct(const C &initNGrid, const ptrdiff_t nGhosts)
        : mTransposed(initNGrid, nGhosts), mHermitianPartners(initNGrid), mNGhosts(nGhosts)
    {
    }

    /** \brief An almost constructor: return a new instance which has a default global FFT layout */
    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsArray<C, NDim>
    static LayoutStruct<NDim> createGlobalFFTLayout(const C &initNGrid)
    {
      LayoutStruct result(initNGrid, 0);
      result.getGlobal().getGlobalSizes()[NDim - 1] = result.getGlobal().getGlobalSizes()[NDim - 1] / 2 + 1;
      result.getLocal().getLocalSizes()[NDim - 1] = result.getGlobalSizes()[NDim - 1];
      return result;
    }

    template <typename T = double> KOKKOS_FORCEINLINE_FUNCTION T getMaxRadius() const
    {
      return getGlobal().template getMaxRadius<T>();
    }

    KOKKOS_FORCEINLINE_FUNCTION
    bool isTransposed() const { return getTransposed().isTransposed(); }

    /** \brief local index in some dimension of the memory layout, goes into its corresponding spatial dimension
     *  in the target memory. No bounds checking!
     */
    template <typename Container, typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void putSpatialLocationFromMemoryIndexInto(Container &target, const IDX... idx) const
    {
      const auto indices = std::tie(idx...);
      constexpr_for<0, NDim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        auto map = getTransposed().getSpatialLocationFromMemoryIndex(std::get<d>(indices), d);
        target[map.atIndex] = map.withValue;
      });
    }

    template <typename Container, typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void putSpatialLocationFromMemoryIndexInto0N(Container &target, const IDX... idx)
        const // Brings back the coordinates between 0 and N-1. Useful for saving and loading for example
    {
      putSpatialLocationFromMemoryIndexInto(target, idx...);
      for (size_t j = 0; j < NDim; ++j)
        if (target[j] < 0) target[j] = target[j] + getGlobal().getGlobalSizes()[j];
    }

    /** \brief Inverse of putSpatialLocationFromMemoryIndexInto: from spatial
     *  coordinate to memory indices, in memory-layout order (that is,
     *  transposed, ready to be applied to `JumpsHolder::getJumpsInMemoryOrder()`.
     */
    template <typename Container, typename... IDX>
      requires VariadicNDIndex<NDim, IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void putMemoryIndexFromSpatialLocationInto(Container &target, const IDX... pos) const
    {
      const auto positions = std::tie(pos...);
      constexpr_for<0, NDim, 1>([&](const auto _d) {
        constexpr size_t d = decltype(_d)::value;
        auto map = getTransposed().getMemoryIndexFromSpatialLocation(std::get<d>(positions), d);
        target[map.atIndex] = map.withValue;
      });
    }

    KOKKOS_FORCEINLINE_FUNCTION
    const Kokkos::Array<ptrdiff_t, NDim> &getGlobalSizes() const { return getGlobal().getGlobalSizes(); }

    template <typename C>
      requires IsArray<C, NDim>
    void setLocalSizes(const C &input)
    {
      Kokkos::Array<ptrdiff_t, NDim> localSizes;
      for (size_t i = 0; i < NDim; ++i)
        localSizes[i] = input[i];
      getTransposed().setLocalSizes(localSizes);
    }

    void setNGhosts(ptrdiff_t nGhosts)
    {
      mNGhosts = nGhosts;
      getTransposed().setNGhosts(nGhosts);
    }

    ptrdiff_t getNGhosts() const { return mNGhosts; }

    Kokkos::Array<ptrdiff_t, NDim> &getLocalSizes() { return getLocal().getLocalSizes(); }
    KOKKOS_FORCEINLINE_FUNCTION
    const Kokkos::Array<ptrdiff_t, NDim> &getLocalSizes() const { return getLocal().getLocalSizes(); }

    KOKKOS_FORCEINLINE_FUNCTION
    const Kokkos::Array<ptrdiff_t, NDim> &getSizesInMemory() const { return getTransposed().getSizesInMemory(); }

    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsArray<C, NDim>
    void setLocalStarts(const C &input)
    {
      getLocal().setLocalStarts(input);
    }
    KOKKOS_FORCEINLINE_FUNCTION
    const Kokkos::Array<ptrdiff_t, NDim> &getLocalStarts() const { return getLocal().getLocalStarts(); }

    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsArray<C, NDim>
    void setTranspositionMap_memoryToGlobalSpace(const C &input)
    {
      getTransposed().setTranspositionMap_memoryToGlobalSpace(input);
    }
    KOKKOS_FORCEINLINE_FUNCTION
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

  private:
    LayoutStructLocalTransposed<NDim> mTransposed;
    /** \brief signed wavenumber and coordinate x = index > n/2 ? index - n : index. Need to provide this n/2 for each
     * dimensions. */
    HermitianPartners<NDim> mHermitianPartners;
    ptrdiff_t mNGhosts;

    KOKKOS_FORCEINLINE_FUNCTION
    LayoutStructLocalTransposed<NDim> &getTransposed() { return mTransposed; }
    KOKKOS_FORCEINLINE_FUNCTION
    LayoutStructLocal<NDim> &getLocal() { return getTransposed().getLocal(); }
    KOKKOS_FORCEINLINE_FUNCTION
    LayoutStructGlobal<NDim> &getGlobal() { return getLocal().getGlobal(); }

    KOKKOS_FORCEINLINE_FUNCTION
    const LayoutStructLocalTransposed<NDim> &getTransposed() const { return mTransposed; }
    KOKKOS_FORCEINLINE_FUNCTION
    const LayoutStructLocal<NDim> &getLocal() const { return getTransposed().getLocal(); }
    KOKKOS_FORCEINLINE_FUNCTION
    const LayoutStructGlobal<NDim> &getGlobal() const { return getLocal().getGlobal(); }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
