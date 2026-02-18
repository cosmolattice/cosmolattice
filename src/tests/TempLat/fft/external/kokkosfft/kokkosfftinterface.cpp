
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#ifdef HAVE_KOKKOSFFT
#include "TempLat/fft/external/kokkosfft/kokkosfftinterface.h"
#include "TempLat/fft/external/kokkosfft/kokkosfftinterface_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::KokkosFFTInterfaceTester> test;
}
#endif
