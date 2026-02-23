#if !defined(TempLat_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTINTERFACE_H) && defined(HAVE_KOKKOSFFT)
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTINTERFACE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/fft/external/kokkosfft/kokkosfftmemorylayout.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftguard.h"

namespace TempLat
{
  /** @brief A class which implements all of FFTLibraryInterface. The larger methods are implemented in classes from
   *which we inherit, in a linear chain: KokkosFFTMemoryLayout and KokkosFFTPlanner.
   *
   *
   * Unit test: ctest -R test-kokkosinterface
   **/
  template <size_t NDim> class KokkosFFTInterface : public KokkosFFTMemoryLayout<NDim>
  {
  public:
    // Put public methods here. These should change very little over time.
    KokkosFFTInterface() = default;

    virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions)
    {
      return std::max((ptrdiff_t)1, nDimensions - 1);
    };

    virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nGridPoints)
    {
      return IntrinsicScales{1., 1};
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

} // namespace TempLat

#endif
