#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWINTERFACE_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWINTERFACE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/fft/external/fftw/fftwguard.h"
#include "TempLat/fft/external/fftw/fftwmemorylayout.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  inline std::shared_ptr<FFTSessionGuard> getFFTWSessionGuard(bool pVerbose = true)
  {
    return std::make_shared<FFTWGuard>(pVerbose);
  }

  /** \brief A class which implements all of FFTLibraryInterface. The larger methods are implemented in classes from
   *which we inherit, in a linear chain: FFTWMemoryLayout and FFTWPlanner.
   *
   *
   * Unit test: make test-fftwinterface
   **/
  template <size_t NDim> class FFTWInterface : public FFTWMemoryLayout<NDim>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    FFTWInterface()
    {
#ifndef NOKOKKOS
      // Sanity check: Kokkos complex should be perfectly compatible with FFTW complex.
      static_assert(sizeof(fftwf_complex) == sizeof(Kokkos::complex<float>));
      static_assert(alignof(fftwf_complex) <= alignof(Kokkos::complex<float>));
      static_assert(sizeof(fftw_complex) == sizeof(Kokkos::complex<double>));
      static_assert(alignof(fftw_complex) <= alignof(Kokkos::complex<double>));
#endif
    }

    virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions) { return 1; };

    virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nDimensions, ptrdiff_t nGridPoints)
    {
      return IntrinsicScales();
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwmemorylayout_test.h"
#endif

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwinterface_test.h"
#endif

#endif
