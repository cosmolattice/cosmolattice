#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWGUARD_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWGUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#ifndef NOFFT
#ifdef HAVE_MPI
#include "fftw3-mpi.h"
#endif
#endif

#ifndef NOFFT
#include "fftw3.h"
#endif

#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/threadsettings.h"

namespace TempLat
{

  /** @brief A class which guards intialization and finalization of FFTW.
   * Only used by MPIGuard, which manually calls its destructor.
   *
   *
   * Unit test: ctest -R test-fftwguard
   **/
  class FFTWGuard : public FFTSessionGuard
  {
  public:
    // Put public methods here. These should change very little over time.

    FFTWGuard(bool verbose = true) : mVerbose(verbose)
    {
#ifndef NOFFT
      if (mVerbose) sayShort << "Calling FFTW local initializations.\n";
      fftw_init_threads();
#ifdef HAVE_FFTFLOAT
      fftwf_init_threads();
#endif
      /* set the default: */

      if (mVerbose) sayShort << "Calling FFTW thread initializations.\n";
      fftw_plan_with_nthreads(static_cast<int>(ThreadSettings::getMaxThreadCount()));
#ifdef HAVE_FFTFLOAT
      fftwf_plan_with_nthreads(static_cast<int>(ThreadSettings::getMaxThreadCount()));
#endif

#ifdef HAVE_MPI

      if (mVerbose) sayShort << "Calling FFTW MPI initializations.\n";

      fftw_mpi_init();
#ifdef HAVE_FFTFLOAT
      fftwf_mpi_init();
#endif

#endif // HAVE_MPI

#endif // NOFFT
    }

    ~FFTWGuard() override
    {
#ifndef NOFFT

      if (mVerbose) sayShort << "Calling FFTW thread finalizations.\n";
      fftw_cleanup_threads();
#ifdef HAVE_FFTFLOAT
      fftwf_cleanup_threads();
#endif

      if (mVerbose) sayShort << "Calling FFTW local finalizations.\n";
      fftw_cleanup();
#ifdef HAVE_FFTFLOAT
      fftwf_cleanup();
#endif

#ifdef HAVE_MPI
      if (mVerbose) sayShort << "Calling FFTW MPI finalizations.\n";
      fftw_mpi_cleanup();
#ifdef HAVE_FFTFLOAT
      fftwf_mpi_cleanup();
#endif
#endif // HAVE_MPI

#endif // NOFFT
    }

    /* delete the copy constructor and copy assignment */
    FFTWGuard(const FFTWGuard &other) = delete;
    FFTWGuard &operator=(const FFTWGuard &other) = delete;
    FFTWGuard &operator=(FFTWGuard &&other) = delete;

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    bool mVerbose;
  };

  inline std::shared_ptr<FFTSessionGuard> getFFTWSessionGuard(bool pVerbose = true)
  {
    return std::make_shared<FFTWGuard>(pVerbose);
  }

} // namespace TempLat

#endif
