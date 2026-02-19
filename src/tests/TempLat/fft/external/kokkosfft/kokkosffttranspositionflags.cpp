
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifdef HAVE_KOKKOSFFT

#include "TempLat/fft/external/kokkosfft/kokkosffttranspositionflags.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct KokkosFFTTranspositionFlagsTester {
    static void Test(TDDAssertion &tdd) { tdd.verify(true); }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::KokkosFFTTranspositionFlagsTester> test;
}

#endif
