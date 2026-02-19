#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTTRANSPOSITIONFLAGS_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTTRANSPOSITIONFLAGS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifndef NOFFT
#ifdef HAVE_KOKKOSFFT
#include <KokkosFFT.hpp>
#endif
#endif

#include "TempLat/fft/fftlibraryinterface.h"

namespace TempLat
{

  /** @brief A class which determines the transpose-flags for the KokkosFFT planners, corresponding to a given layout.
   *
   * Unit test: ctest -R test-kokkosffttranspositionflags
   **/
  template <size_t NDim> class KokkosFFTTranspositionFlags
  {
  public:
    // Put public methods here. These should change very little over time.
    KokkosFFTTranspositionFlags(const FFTLayoutStruct<NDim> &layout)
    {
      // bool transposedC = layout.fourierSpace.isTransposed();
      // bool transposedR = layout.configurationSpace.isTransposed();

      // mFlagC2R = (transposedC ? KOKKOSFFT_TRANSPOSED_IN : 0) | (transposedR ? KOKKOSFFT_TRANSPOSED_OUT : 0);

      // mFlagR2C = (transposedC ? KOKKOSFFT_TRANSPOSED_OUT : 0) | (transposedR ? KOKKOSFFT_TRANSPOSED_IN : 0);
    }

    unsigned int c2r() { return mFlagC2R; }
    unsigned int r2c() { return mFlagR2C; }

    friend std::ostream &operator<<(std::ostream &ostream, const KokkosFFTTranspositionFlags &tr)
    {
      ostream << "C2R : "
              //       << ((tr.mFlagC2R & KOKKOSFFT_TRANSPOSED_IN)    ? " transposed in"
              //          : (tr.mFlagC2R & KOKKOSFFT_TRANSPOSED_OUT) ? "transposed out"
              //                                                 : "not transposed")
              << ".\n";
      ostream << "R2C : "
              //    << ((tr.mFlagR2C & KOKKOSFFT_TRANSPOSED_IN)    ? " transposed in"
              //       : (tr.mFlagR2C & KOKKOSFFT_TRANSPOSED_OUT) ? "transposed out"
              //                                              : "not transposed")
              << ".\n";
      return ostream;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    unsigned int mFlagC2R;
    unsigned int mFlagR2C;
  };

} // namespace TempLat

#endif
