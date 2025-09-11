#ifndef TEMPLAT_PARALLEL_KOKKOS_SESSION_KOKKOSGUARD_TEST_H
#define TEMPLAT_PARALLEL_KOKKOS_SESSION_KOKKOSGUARD_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

inline void TempLat::KokkosGuard::Test(TempLat::TDDAssertion &tdd)
{

  if (KokkosGuard::InstanceCounter() < 1) {
    KokkosGuard guard(0, NULL, true);
  } else {
    /* there is an instance of KokkosGuard in the calling main, which is a good thing. Then we can test if the multiple
     * instantiation protection works. */
    tdd.verify(Throws<KokkosGuardInstantiationException>([]() { KokkosGuard guard(0, NULL, true); }));
  }
}

#endif
