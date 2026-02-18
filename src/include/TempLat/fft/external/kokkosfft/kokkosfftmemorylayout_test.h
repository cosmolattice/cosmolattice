#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTMEMORYLAYOUT_TEST_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_KOKKOSFFTMEMORYLAYOUT_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/mpi/comm/mpidomainsplit.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftinterface.h"

inline void TempLat::KokkosFFTMemoryLayoutTester::Test(TempLat::TDDAssertion &tdd)
{
  // ...
  tdd.verify(true);
}

#endif

