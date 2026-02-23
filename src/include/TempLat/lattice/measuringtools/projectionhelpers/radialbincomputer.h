#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALBINCOMPUTER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALBINCOMPUTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which returns an integer bin for a given fp value.
   *
   * Unit test: ctest -R test-radialbincomputer
   **/
  class RadialBinComputer
  {
  public:
    RadialBinComputer(double minVal, double maxVal, ptrdiff_t nBins)
        : mMinVal(minVal), mMaxVal(maxVal), mRange(mMaxVal - mMinVal), mNBins(nBins), mHighestBin(nBins - 1)
    {
      if (mRange <= 0) mRange = 1;
    }

    /** @brief Call this for your value, receive a bin index in return. */
    DEVICE_FUNCTION
    ptrdiff_t operator()(double value) const
    {
      ptrdiff_t bin = static_cast<ptrdiff_t>(device::round(static_cast<double>(mNBins) * (value - mMinVal) / mRange));
      return device::min(mHighestBin, device::max(ptrdiff_t(0), bin));
    }

    template <typename T> void setCentralBinBounds(std::vector<T> &res)
    {
      res = std::vector<T>(mNBins);
      T steps = mRange / static_cast<double>(mNBins);
      for (ptrdiff_t i = 0; i < mNBins; ++i) {
        res[i] = mMinVal + i * steps;
      }
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    double mMinVal;
    double mMaxVal;
    double mRange;
    ptrdiff_t mNBins;
    ptrdiff_t mHighestBin;
  };
} // namespace TempLat

#endif
