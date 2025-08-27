#if !defined(TempLat_FFT_EXTERNAL_HEFFTE_HEFFTEINTERFACE_H) && !defined(NOHEFFTE)
#define TEMPLAT_FFT_EXTERNAL_HEFFTE_HEFFTEINTERFACE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/heffte/hefftememorylayout.h"
#include "TempLat/fft/external/heffte/heffteguard.h"

namespace TempLat
{
  /** \brief A class which implements all of FFTLibraryInterface. The larger methods are implemented in classes from
   *which we inherit, in a linear chain: HEFFTEMemoryLayout and HEFFTEPlanner.
   *
   *
   * Unit test: make test-heffteinterface
   **/
  template <size_t NDim> class HEFFTEInterface : public HEFFTEMemoryLayout<NDim>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    HEFFTEInterface() {}

    virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions)
    {
      return std::max((ptrdiff_t)1, nDimensions - 1);
    };

    virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nDimensions, ptrdiff_t nGridPoints)
    {
      return IntrinsicScales();
    }

    virtual std::shared_ptr<FFTSessionGuard> getSessionGuard(bool pVerbose = true)
    {
      return std::make_shared<HEFFTEGuard>(pVerbose);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
