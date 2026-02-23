#ifndef TEMPLAT_FFT_FFTNORMALIZATION_H
#define TEMPLAT_FFT_FFTNORMALIZATION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/memory/memorylayouts/fftlayoutstruct.h"
#include "TempLat/util/almostequal.h"
#include "TempLat/lattice/memory/memoryblock.h"

#include "TempLat/parallel/device_iteration.h"

namespace TempLat
{
  /** @brief A class which applies the normalization to a block of memory after C2R and R2C fft's.
   * You can choose between:
   * fourier: C2R: 1/N, R2C: 1,
   * mixed:   C2R: 1/sqrt(N), R2C: 1/sqrt(N),
   * config:  C2R: 1, R2C: 1/N,
   *
   * Unit test: ctest -R test-fftnormalization
   **/
  template <size_t NDim> class FFTNormalization
  {
  public:
    // Put public methods here. These should change very little over time.
    FFTNormalization(FFTLayoutStruct<NDim> layout) : mLayout(layout), mN(1), mNorm(1), mType(0)
    {
      // we always normalize the entire memory, including padding.
      for (auto &&it : mLayout.configurationSpace.getGlobalSizes())
        mN *= it;
      setToConfigType();
    }

    static constexpr int cConfigType = 0;
    static constexpr int cFourierType = 1;
    static constexpr int cMixedType = 2;

    void setToConfigType()
    {
      mNorm = 1. / (double)mN;
      mType = cConfigType;
    }
    void setToFourierType()
    {
      mNorm = 1. / (double)mN;
      mType = cFourierType;
    }
    void setToMixedType()
    {
      mNorm = 1. / std::sqrt((double)mN);
      mType = cMixedType;
    }

    template <typename T> inline void c2r(MemoryBlock<NDim, T> mBlock, T scale)
    {
      T intrinsicScale = (T)(mLayout.getIntrinsicScales().c2r);
      switch (mType) {
      case cConfigType:
      case cMixedType:
        apply(mBlock, (T)(scale * mNorm * intrinsicScale));
        break;
      case cFourierType:
        if (!AlmostEqual(intrinsicScale, 1)) apply(mBlock, intrinsicScale);
      default:
        break;
      }
    }

    template <typename T> inline void r2c(MemoryBlock<NDim, T> mBlock, T scale)
    {
      T intrinsicScale = (T)(mLayout.getIntrinsicScales().r2c);
      switch (mType) {
      case cFourierType:
      case cMixedType:
        apply(mBlock, (T)(scale * mNorm * intrinsicScale));
        break;
      case cConfigType:
        if (!AlmostEqual(intrinsicScale, 1)) apply(mBlock, intrinsicScale);
      default:
        break;
      }
    }

    template <typename T> inline void apply(MemoryBlock<NDim, T> &mBlock, T norm)
    {
      auto block_view = mBlock.getRawView();
      auto functor = DEVICE_LAMBDA(const device::IdxArray<1> &i) { block_view(i[0]) *= norm; };
      device::iteration::foreach ("FFTNormalization", device::IdxArray<1>{0},
                                  device::IdxArray<1>{(device::Idx)mBlock.size()}, functor);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    FFTLayoutStruct<NDim> mLayout;
    ptrdiff_t mN;
    double mNorm;
    int mType;
  };

} // namespace TempLat

#endif
