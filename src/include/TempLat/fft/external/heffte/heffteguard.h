#ifndef TEMPLAT_FFT_EXTERNAL_HEFFTE_GUARD_H
#define TEMPLAT_FFT_EXTERNAL_HEFFTE_GUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"

#ifndef NOMPI
#include <mpi.h>
#endif

#ifndef NOFFT
#ifndef NOMPI
#ifndef NOHEFFTE
#include <heffte.h>
#endif
#endif
#endif

namespace TempLat
{

  /** \brief
   * HEFFTE abstracts away all of the initialization and finalization of its dependencies. Therefore, this guard does
   * nothing. It is only here for symmetry with FFTW and PFFT.
   *
   * Unit test: make test-heffteguard
   **/
  class HEFFTEGuard : public FFTSessionGuard
  {
  public:
    HEFFTEGuard([[maybe_unused]]
                bool verbose = true)
    {
      if (verbose) {
        sayMPI << "HEFFTEGuard: HEFFTE does not require initialization or finalization. This guard does nothing.\n";
      }
    }

    ~HEFFTEGuard() {}

    /* delete the copy constructor and copy assignment */
    HEFFTEGuard(const HEFFTEGuard &other) = delete;
    HEFFTEGuard &operator=(const HEFFTEGuard &other) = delete;
    HEFFTEGuard &operator=(HEFFTEGuard &&other) = delete;

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  inline std::shared_ptr<FFTSessionGuard> getHEFFTESessionGuard(bool pVerbose = true)
  {
    return std::make_shared<HEFFTEGuard>(pVerbose);
  }
} // namespace TempLat

#endif
