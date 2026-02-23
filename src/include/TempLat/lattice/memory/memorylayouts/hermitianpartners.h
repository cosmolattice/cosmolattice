#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/hash/keccakhashbareclass.h"
#include "TempLat/util/powr.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianredundancy.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianvalueaccounting.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  enum class HermitianPartnersMode { none, fftw };

  /** @brief An almost abstract class which your FFT library must implement, which maps the redundant entries in
   *  the complex representation of the FFT of your real values data, to their hermitian-conjugate partners.
   *
   * Unit test: ctest -R test-hermitianpartners
   **/
  template <size_t NDim> class HermitianPartners
  {
  public:
    // Put public methods here. These should change very little over time.
    DEVICE_FUNCTION HermitianPartners(const device::IdxArray<NDim> &initNGrid) : mode(HermitianPartnersMode::none)
    {
      for (size_t i = 0; i < NDim; ++i) {
        mNGrid[i] = initNGrid[i];
        mSignConversionMidpoint[i] = mNGrid[i] / 2;
      }
    }

    /** @brief For testing purposes: track which entries in the layout carry redundant information, and if so, what
     * information. The default implementation returns HermitianRedundancy::none, which you could (uselessly) use for
     * configuration-space layouts.
     */
    template <typename Container>
      requires requires(Container c) { c[NDim - 1]; }
    DEVICE_FUNCTION HermitianRedundancy qualify(const Container &globalCoordinate) const
    {
      if (mode == HermitianPartnersMode::none) {
        // see below
      } else if (mode == HermitianPartnersMode::fftw) {
        constexpr size_t lastDim = NDim - 1;

        HermitianRedundancy result = HermitianRedundancy::none;
        if ((globalCoordinate[lastDim] == 0) || globalCoordinate[lastDim] == mSignConversionMidpoint[lastDim]) {
          bool isRealValued = true;
          bool leadingZeros = true;
          bool isNegativePartner = false;
          for (size_t i = 0; i < lastDim; ++i) {
            isRealValued =
                isRealValued && (globalCoordinate[i] == 0 || globalCoordinate[i] == mSignConversionMidpoint[i]);
            /* if the first non-zero coordinate is negative, we are a negative partner. */
            if (leadingZeros && globalCoordinate[i] < 0) isNegativePartner = true;
            /* after testing for negative partnership, update leadingZeros. */
            leadingZeros =
                leadingZeros && (globalCoordinate[i] == 0 || globalCoordinate[i] == mSignConversionMidpoint[i]);
          }
          result = isRealValued        ? HermitianRedundancy::realValued
                   : isNegativePartner ? HermitianRedundancy::negativePartner
                                       : HermitianRedundancy::positivePartner;
        }
        return result;
      }
      return HermitianRedundancy::none;
    }

    /** @brief If the entry at your input globalCoordinate has a partner which is its hermitian conjugate,
     *  then return the coordinates to that partner. Otherwise return the input. No bounds checking!
     */
    template <typename Container1, typename Container2>
      requires requires(Container1 c, Container2 d) {
        c[NDim - 1];
        d[NDim - 1];
      }
    DEVICE_FUNCTION HermitianRedundancy putHermitianPartner(const Container1 &globalCoordinate,
                                                            Container2 &target) const
    {
      if (mode == HermitianPartnersMode::none) {
        // see below
      } else if (mode == HermitianPartnersMode::fftw) {
        auto q = qualify(globalCoordinate);
        if (q != HermitianRedundancy::negativePartner) {
          for (size_t i = 0; i < NDim; ++i)
            target[i] = globalCoordinate[i];
        } else {
          for (size_t i = 0; i < NDim; ++i) {
            target[i] = globalCoordinate[i] == 0 || globalCoordinate[i] == mSignConversionMidpoint[i]
                            ? globalCoordinate[i]
                            : -globalCoordinate[i];
          }
        }
        return q;
      }
      // The default implementation copies the input coordinates, which you could (uselessly) use for
      // configuration-space layouts.
      for (size_t i = 0; i < NDim; ++i)
        target[i] = globalCoordinate[i];
      return HermitianRedundancy::positivePartner;
    }

    /** @brief Compute the number of unique / independent real and imaginary floating point values in a memory layout.
     */
    DEVICE_FUNCTION
    HermitianValueAccounting getNumberOfIndependentValues() const
    {
      if (mode == HermitianPartnersMode::fftw) {
        /* How do we get here? With the  layout in FFTW  we have N x N x ... x N/2+1 complex values -> i.e. naively 1
         * real and 1 imaginary value for each. However, half of the entries in the last dim's [N/2] are redundant, as
         * are half of the entries in the last dim's [0], i.e. together N x N x ... x 2 entries. Moreover,
         * out of the independent half, all values whose coordinate consists of only 0's and N/2's are real-valued, so
         * they don't go into the imaginary count.
         */

        device::Idx nGridVertices = 1;

        /* mNGrid always has the shape of the real-valued input problem,
         * not the resulting r2c complex layout.
         */
        for (const auto &it : mNGrid)
          nGridVertices *= it;

        device::Idx nComplexVerticesAfterR2C = nGridVertices / mNGrid[NDim - 1] * (mNGrid[NDim - 1] / 2 + 1);

        device::Idx hermitianSymmetricEntries = nGridVertices / mNGrid[NDim - 1];

        device::Idx imaginary = nComplexVerticesAfterR2C - hermitianSymmetricEntries - powr<NDim - 1>(2);

        /* 2^ND-1 is the number of real-valued entries, at {{ 0, 0, N/2, N/2, ...}}. */
        device::Idx real = imaginary + powr<NDim>(2);

        return {real, imaginary};
      } else // if (mode == HermitianPartnersMode::none)
      {
        device::Idx vol = 1;
        for (device::Idx x : mNGrid)
          vol *= x;
        return {vol, 0};
      }
    }

    std::string toString() const
    {
      if (mode == HermitianPartnersMode::none) {
        // See below
      } else if (mode == HermitianPartnersMode::fftw) {
        return "FFTW HermitianRedundancy describer.";
      }
      return "Default HermitianRedundancy describer (configuration space -> no hermitian symmetry).";
    }

    friend std::ostream &operator<<(std::ostream &ostream, const HermitianPartners &hp)
    {
      ostream << (&hp)->toString();
      return ostream;
    }

    template <size_t NDim2> friend bool operator==(const HermitianPartners<NDim> &a, const HermitianPartners<NDim2> &b)
    {
      if constexpr (NDim != NDim2)
        return false;
      else
        return a.mNGrid == b.mNGrid;
    }

    HermitianPartnersMode getMode() const { return mode; }
    void setMode(HermitianPartnersMode new_mode) { this->mode = new_mode; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    device::IdxArray<NDim> mNGrid;
    HermitianPartnersMode mode;
    device::IdxArray<NDim> mSignConversionMidpoint;
  };

} // namespace TempLat

#endif
