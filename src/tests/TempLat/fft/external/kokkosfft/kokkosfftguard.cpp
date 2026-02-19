
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/fft/external/kokkosfft/kokkosfftguard.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct KokkosFFTGuardTester {
    static void Test(TDDAssertion &tdd)
    {
      /* ONLY when single unit, imperatively, because
       * the internals can only be called once per process.
       * If we are not the only test, chances are that the actual
       * mpiguard has an instance of us.
       */
      if (TDDRegister::isSingleUnitTest()) {
        KokkosFFTGuard guard;
      }

      tdd.verify(true);
    }
  };

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::KokkosFFTGuardTester> test;
}
