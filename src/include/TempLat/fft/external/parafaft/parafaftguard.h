#ifndef TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTGUARD_H
#define TEMPLAT_FFT_EXTERNAL_PARAFAFT_PARAFAFTGUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2026

#include "TempLat/fft/fftlibraryinterface.h"

#ifdef HAVE_MPI
#include <mpi.h>
#endif

namespace TempLat
{

  /** @brief RAII guard for parafaft initialization/cleanup.
   *
   * Parafaft is a header-only library that uses FFTW internally.
   * Since FFTW initialization is handled by FFTWGuard, this guard
   * only needs to exist to satisfy the interface - no actual
   * initialization or cleanup is required.
   *
   * Unit test: ctest -R test-parafaftguard
   **/
  class ParafaftGuard : public FFTSessionGuard
  {
  public:
    ParafaftGuard(bool verbose = true) : mVerbose(verbose)
    {
      if (mVerbose) {
      }; /* just for the compiler warnings */
#ifdef HAVE_MPI
#ifdef HAVE_PARAFAFT
#ifdef CHECKBOUNDS
      if (mVerbose) sayShort << "Parafaft guard constructed (header-only, no initialization needed).\n";
#endif
#endif
#endif
    }

    ~ParafaftGuard()
    {
#ifdef HAVE_MPI
#ifdef HAVE_PARAFAFT
#ifdef CHECKBOUNDS
      if (mVerbose) sayShort << "Parafaft guard destructed (no cleanup needed).\n";
#endif
#endif
#endif
    }

    /* delete the copy constructor and copy assignment */
    ParafaftGuard(const ParafaftGuard &other) = delete;
    ParafaftGuard &operator=(const ParafaftGuard &other) = delete;
    ParafaftGuard &operator=(ParafaftGuard &&other) = delete;

  private:
    bool mVerbose;
  };
} // namespace TempLat

#endif
