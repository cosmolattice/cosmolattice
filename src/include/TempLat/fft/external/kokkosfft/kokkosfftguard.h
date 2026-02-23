#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_GUARD_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_GUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/fft/fftlibraryinterface.h"

#ifdef HAVE_MPI
#include <mpi.h>
#endif

#ifndef NOFFT
#ifdef HAVE_KOKKOSFFT
#include <KokkosFFT.hpp>
#endif
#endif

namespace TempLat
{
  /** @brief KokkosFFT abstracts away all of the initialization and finalization of its dependencies. Therefore, this
   * guard does nothing. It is only here for symmetry with FFTW.
   *
   * Unit test: ctest -R test-kokkosguard
   **/
  class KokkosFFTGuard : public FFTSessionGuard
  {
  public:
    KokkosFFTGuard([[maybe_unused]]
                   bool verbose = true)
    {
      if (verbose) {
        sayMPI
            << "KokkosFFTGuard: KokkosFFT does not require initialization or finalization. This guard does nothing.\n";
      }
    }

    ~KokkosFFTGuard() override = default;

    /* delete the copy constructor and copy assignment */
    KokkosFFTGuard(const KokkosFFTGuard &other) = delete;
    KokkosFFTGuard &operator=(const KokkosFFTGuard &other) = delete;
    KokkosFFTGuard &operator=(KokkosFFTGuard &&other) = delete;
  };

  inline std::shared_ptr<FFTSessionGuard> getKokkosFFTSessionGuard(bool pVerbose = true)
  {
    return std::make_shared<KokkosFFTGuard>(pVerbose);
  }

} // namespace TempLat

#endif
