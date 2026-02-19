#ifndef TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_GUARD_TEST_H
#define TEMPLAT_FFT_EXTERNAL_KOKKOSFFT_GUARD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

// Verified, test passed: this does not compile, just the way we like it.
// inline void ThisShouldNotCompile() { TempLat::KokkosFFTGuard guard; }

inline void TempLat::KokkosFFTGuardTester::Test(TempLat::TDDAssertion &tdd)
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

#endif
