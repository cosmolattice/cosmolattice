#ifndef TEMPLAT_FFT_EXTERNAL_PFFT_FFTWGUARD_H
#define TEMPLAT_FFT_EXTERNAL_PFFT_FFTWGUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#ifndef NOFFT
#ifdef HAVE_MPI
#ifdef HAVE_PFFT
#include "pfft.h"
#endif
#endif
#endif

namespace TempLat
{

  /** @brief A class which guards intialization and finalization of PFFT. Replace this if you replace PFFT with an
   *alternative library. Only used by MPIGuard, which manually calls its destructor.
   *
   * Unit test: ctest -R test-pfftguard
   **/
  class PFFTGuard : public FFTSessionGuard
  {
  public:
    PFFTGuard(bool verbose = true) : mVerbose(verbose)
    {
      if (mVerbose) {
      }; /* just for the compiler warnings */
#ifdef HAVE_MPI
#ifdef HAVE_PFFT
#ifdef TEMPLATTEST
      if (mVerbose) sayShort << "Calling PFFT initializations.\n";
#endif
      pfft_init();
#ifdef HAVE_FFTFLOAT
      pfftf_init();
#endif
#endif
#endif
    }

    ~PFFTGuard()
    {
#ifdef HAVE_MPI
#ifdef HAVE_PFFT
#ifdef TEMPLATTEST
      if (mVerbose) sayShort << "Calling PFFT finalizations.\n";
#endif
      pfft_cleanup();
#ifdef HAVE_FFTFLOAT
      pfftf_cleanup();
#endif
#endif
#endif
    }

    /* delete the copy constructor and copy assignment */
    PFFTGuard(const PFFTGuard &other) = delete;
    PFFTGuard &operator=(const PFFTGuard &other) = delete;
    PFFTGuard &operator=(PFFTGuard &&other) = delete;

  private:
    bool mVerbose;

#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  inline std::shared_ptr<FFTSessionGuard> getPFFTSessionGuard(bool pVerbose = true)
  {
    return std::make_shared<PFFTGuard>(pVerbose);
  }
} // namespace TempLat

#endif
