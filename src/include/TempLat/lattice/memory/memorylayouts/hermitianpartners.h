#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <vector>
#include <cmath>

#include "TempLat/util/hash/keccakhashbareclass.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianredundancy.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianvalueaccounting.h"
#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/isarray.h"

namespace TempLat
{
  enum class HermitianPartnersMode { none, fftw };

  /** \brief An almost abstract class which your FFT library must implement, which maps the redundant entries in
   *  the complex representation of the FFT of your real values data, to their hermitian-conjugate partners.
   *
   * Unit test: make test-hermitianpartners
   **/
  template <size_t NDim> class HermitianPartners
  {
  public:
    /* Put public methods here. These should change very little over time. */
    template <typename C = std::array<ptrdiff_t, NDim>>
      requires IsArray<C, NDim>
    KOKKOS_FUNCTION HermitianPartners(const C &initNGrid) : mode(HermitianPartnersMode::none)
    {
      for (size_t i = 0; i < NDim; ++i) {
        mNGrid[i] = initNGrid[i];
        mSignConversionMidpoint[i] = mNGrid[i] / 2;
      }
    }

    /** \brief For testing purposes: track which entries in the layout carry redundant information, and if so, what
     * information. The default implementation returns HermitianRedundancy::none, which you could (uselessly) use for
     * configuration-space layouts.
     */
    KOKKOS_FUNCTION
    HermitianRedundancy qualify(const Kokkos::Array<ptrdiff_t, NDim> &globalCoordinate) const
    {
      if (mode == HermitianPartnersMode::none) {
        // see below
      } else if (mode == HermitianPartnersMode::fftw) {
        size_t lastDim = NDim - 1;

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

    /** \brief If the entry at your input globalCoordinate has a partner which is its hermitian conjugate,
     *  then return the coordinates to that partner. Otherwise return the input. No bounds checking!
     */
    KOKKOS_FUNCTION
    HermitianRedundancy putHermitianPartner(const Kokkos::Array<ptrdiff_t, NDim> &globalCoordinate,
                                            Kokkos::Array<ptrdiff_t, NDim> &target) const
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

    /** \brief Compute the number of unique / independent real and imaginary floating point values in a memory layout.
     */
    KOKKOS_FUNCTION
    HermitianValueAccounting getNumberOfIndependentValues() const
    {
      if (mode == HermitianPartnersMode::none) {
        ptrdiff_t vol = 1;
        for (ptrdiff_t x : mNGrid)
          vol *= x;
        return HermitianValueAccounting(vol, 0);
      } else if (mode == HermitianPartnersMode::fftw) {
        /* How do we get here? Well, hermitian redundant layout in FFTW is put in
         * N x N x ... x N/2+1 complex values -> naively 1 real and 1 imaginary for each.
         * Half of the entries in the last dim's [N/2] are redundant,
         * as are half of the entries in the last dim's [0].
         * N x N x ... x 2 are the last dim's [N/2] and [0].
         * Moreover, out of the independent half, all values whose coordinate consists
         * of only 0's and N/2's are real-valued, so they don't go into the imaginary count.
         */

        ptrdiff_t nGridVertices = 1;

        /* mNGrid always has the shape of the real-valued input problem,
         * not the resulting r2c complex layout.
         */
        for (const auto &it : mNGrid)
          nGridVertices *= it;

        ptrdiff_t nComplexVerticesAfterR2C = nGridVertices / mNGrid.back() * (mNGrid.back() / 2 + 1);

        ptrdiff_t hermitianSymmetricEntries = nGridVertices / mNGrid.back();

        ptrdiff_t imaginary = nComplexVerticesAfterR2C - hermitianSymmetricEntries - pow<NDim - 1>(2);

        /* 2^ND-1 is the number of real-valued entries, at {{ 0, 0, N/2, N/2, ...}}. */
        ptrdiff_t real = imaginary + pow<NDim>(2);

        return HermitianValueAccounting(real, imaginary);
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
    std::array<ptrdiff_t, NDim> mNGrid;
    HermitianPartnersMode mode;
    Kokkos::Array<ptrdiff_t, NDim> mSignConversionMidpoint;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/hermitianpartners_test.h"
#endif

#endif
