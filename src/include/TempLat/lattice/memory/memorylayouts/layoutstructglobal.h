#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTGLOBAL_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTGLOBAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

namespace TempLat
{

  // Forward declaration of LayoutStruct template
  template <size_t NDim> struct LayoutStruct;

  /** @brief A class which holds some memory informations.
   *
   *
   * Unit test: ctest -R test-layoutstructglobal
   **/
  template <size_t _NDim> class LayoutStructGlobal
  {
  public:
    // Put public methods here. These should change very little over time.
    static constexpr size_t NDim = _NDim;

    LayoutStructGlobal(const device::IdxArray<NDim> &initNGrid)
    {
      for (size_t i = 0; i < NDim; ++i) {
        mGlobalSizes[i] = initNGrid[i];
        mSignConversionMidpoint[i] = mGlobalSizes[i] / 2;
      }
    }

    DEVICE_FORCEINLINE_FUNCTION
    device::IdxArray<NDim> &getGlobalSizes() { return mGlobalSizes; }
    DEVICE_FORCEINLINE_FUNCTION
    const device::IdxArray<NDim> &getGlobalSizes() const { return mGlobalSizes; }

    /** returns the largest possible distance from the origin. mSignConversionMidpoint holds the
     *  maximum value of each dimension. Check that in localIndexToGlobalCoordinate.
     */
    template <typename T = double> DEVICE_FORCEINLINE_FUNCTION T getMaxRadius() const
    {
      T r2 = 0;
      for (auto &&it : mSignConversionMidpoint)
        r2 += it * it;
      return device::sqrt(r2);
    }

    /** @brief For both configuration and fourier space, the index values are not the same as coordinate
     *  values. Assuming periodic boundary conditions, we get that always c = i > half ? i - N : i;
     *  Don't mix up the arguments! Does not do transposition, so input pre-transposed dimension!
     */
    DEVICE_FORCEINLINE_FUNCTION
    device::Idx memoryIndexToSpatialCoordinate(device::Idx index, device::Idx dimension) const
    {
      const device::Idx &tSize = mSignConversionMidpoint[dimension];
      return index > tSize ? index - mGlobalSizes[dimension] : index;
    }

    /** @brief Inverse of memoryIndexToSpatialCoordinate: get memory from position. */
    DEVICE_FORCEINLINE_FUNCTION
    device::Idx spatialCoordinateToMemoryIndex(device::Idx position, device::Idx dimension) const
    {
      return (position >= 0 ? position : position + mGlobalSizes[dimension]);
    }

    friend struct LayoutStruct<NDim>;

    template <size_t d2> friend bool operator==(const LayoutStructGlobal<NDim> &a, const LayoutStructGlobal<d2> &b)
    {
      if constexpr (NDim != d2)
        return false;
      else {
        bool result = a.mGlobalSizes.size() == b.mGlobalSizes.size() &&
                      a.mSignConversionMidpoint.size() == b.mSignConversionMidpoint.size();

        for (size_t i = 0; i < a.mGlobalSizes.size(); ++i) {
          result = result && a.mGlobalSizes[i] == b.mGlobalSizes[i];
          result = result && a.mSignConversionMidpoint[i] == b.mSignConversionMidpoint[i];
        }
        return result;
      }
    }

    friend std::ostream &operator<<(std::ostream &ostream, const LayoutStructGlobal &ls)
    {
      ostream << "  GlobalSizes: " << ls.mGlobalSizes << "\n"
              << "  SignConversionMidpoint: " << ls.mSignConversionMidpoint << "\n";
      return ostream;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    device::IdxArray<NDim> mGlobalSizes;
    device::IdxArray<NDim> mSignConversionMidpoint;
  };

} // namespace TempLat

#endif
