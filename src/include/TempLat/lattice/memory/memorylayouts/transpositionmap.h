#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/exception.h"

namespace TempLat
{
  MakeException(TranspositionMapOutOfBounds);

  /** @brief A class which maps between two orderings.
   *
   * Unit test: ctest -R test-transpositionmap
   **/
  template <size_t NDim> class TranspositionMap
  {
  public:
    /** @brief Default constructor: untransposed, both maps are a linear range. */
    TranspositionMap() : mFromAtoB{}, mFromBtoA{}
    {
      for (size_t i = 0; i < NDim; ++i) {
        mFromAtoB[i] = i;
        mFromBtoA[i] = i;
      }
    }

    static constexpr size_t size() { return NDim; }

    DEVICE_FORCEINLINE_FUNCTION
    device::Idx getForward(device::Idx index) const { return mFromAtoB[index]; }
    DEVICE_FORCEINLINE_FUNCTION
    device::Idx getInverse(device::Idx index) const { return mFromBtoA[index]; }

    /** @brief Provide your forward mapping, which will be the new output of getForward. */
    void setMap(const device::IdxArray<NDim> &input)
    {
      for (size_t i = 0; i < NDim; ++i)
        mFromAtoB[i] = input[i];

      for (device::Idx i = 0, iEnd = mFromAtoB.size(); i < iEnd; ++i) {
        if (mFromAtoB[i] < 0 || mFromAtoB[i] > iEnd - 1)
          throw TranspositionMapOutOfBounds("Your map has entries that go beyond the size of the map:", input);
        mFromBtoA[mFromAtoB[i]] = i;
      }
    }

    DEVICE_FORCEINLINE_FUNCTION
    bool isUntransposed() const
    {
      bool untransposed = true;
      for (device::Idx i = 0, iEnd = mFromAtoB.size(); i < iEnd; ++i) {
        untransposed = untransposed && mFromAtoB[i] == i && mFromBtoA[i] == i;
      }
      return untransposed;
    }

    DEVICE_FORCEINLINE_FUNCTION
    bool isTransposed() const { return !isUntransposed(); }

    friend bool operator==(const TranspositionMap &a, const TranspositionMap &b)
    {
      bool equal = a.size() == b.size();
      for (device::Idx i = 0, iEnd = a.size(); i < iEnd && equal; ++i) {
        equal = equal && a.getForward(i) == b.getForward(i) && a.getInverse(i) == b.getInverse(i);
      }
      return equal;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const TranspositionMap &a)
    {
      ostream << " forward " << a.mFromAtoB << ", inverse " << a.mFromBtoA;
      return ostream;
    }

  private:
    /* From C to D means that entry at position 0 in C is at position mFromCtoD[0] in D. */
    device::IdxArray<NDim> mFromAtoB;
    device::IdxArray<NDim> mFromBtoA;
  };

} // namespace TempLat

#endif
