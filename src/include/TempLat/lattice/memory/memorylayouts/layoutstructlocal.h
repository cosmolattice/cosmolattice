#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructglobal.h"
#include "TempLat/util/isarray.h"

namespace TempLat
{
  MakeException(LayoutStructLocalSizeException);

  /** \brief A class which
   *  localSizes: the local size in each dimension.
   *  localStart: the coordinate of the first entry in local memory, in the global coordinate space.
   *
   *
   * Unit test: make test-layoutstructlocal
   **/
  template <size_t _NDim> class LayoutStructLocal
  {
  public:
    static constexpr size_t NDim = _NDim;

    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsArray<C, NDim>
    LayoutStructLocal(const C &initNGrid) : mGlobal(initNGrid), mLocalStarts{}
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalSizes[i] = initNGrid[i];
    }

    KOKKOS_FORCEINLINE_FUNCTION
    LayoutStructGlobal<NDim> &getGlobal() { return mGlobal; }
    KOKKOS_FORCEINLINE_FUNCTION
    const LayoutStructGlobal<NDim> &getGlobal() const { return mGlobal; }

    template <typename T = ptrdiff_t> void setLocalSizes(const Kokkos::Array<T, NDim> &input)
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalSizes[i] = input[i];
    }
    template <typename T = ptrdiff_t> void setLocalSizes(const std::array<T, NDim> &input)
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalSizes[i] = input[i];
    }
    KOKKOS_FORCEINLINE_FUNCTION
    Kokkos::Array<ptrdiff_t, NDim> &getLocalSizes() { return mLocalSizes; }
    KOKKOS_FORCEINLINE_FUNCTION
    const Kokkos::Array<ptrdiff_t, NDim> &getLocalSizes() const { return mLocalSizes; }

    template <typename T = ptrdiff_t> void setLocalStarts(const Kokkos::Array<T, NDim> &input)
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalStarts[i] = input[i];
    }
    template <typename T = ptrdiff_t> void setLocalStarts(const std::array<T, NDim> &input)
    {
      for (size_t i = 0; i < NDim; ++i)
        mLocalStarts[i] = input[i];
    }
    KOKKOS_FORCEINLINE_FUNCTION
    Kokkos::Array<ptrdiff_t, NDim> &getLocalStarts() { return mLocalStarts; }
    KOKKOS_FORCEINLINE_FUNCTION
    const Kokkos::Array<ptrdiff_t, NDim> &getLocalStarts() const { return mLocalStarts; }

    /** \brief For both configuration and fourier space, the index values are not the same as coordinate
     *  values. Assuming periodic boundary conditions, we get that always c = i > half ? i - N : i;
     *  Don't mix up the arguments! Does not do transposition, so input pre-transposed dimension!
     */
    KOKKOS_FORCEINLINE_FUNCTION
    ptrdiff_t memoryIndexToSpatialCoordinate(ptrdiff_t index, ptrdiff_t dimension) const
    {
      return mGlobal.memoryIndexToSpatialCoordinate(index + mLocalStarts[dimension], dimension);
    }

    /** \brief Inverse of memoryIndexToSpatialCoordinate: get memory from position. */
    KOKKOS_FORCEINLINE_FUNCTION
    ptrdiff_t spatialCoordinateToMemoryIndex(ptrdiff_t position, ptrdiff_t dimension) const
    {
      return mGlobal.spatialCoordinateToMemoryIndex(position, dimension) - mLocalStarts[dimension];
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
              << "  LocalStarts: " << ls.mLocalStarts << "\n";
      return ostream;
    }

  private:
    LayoutStructGlobal<NDim> mGlobal;
    Kokkos::Array<ptrdiff_t, NDim> mLocalSizes;
    Kokkos::Array<ptrdiff_t, NDim> mLocalStarts;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
