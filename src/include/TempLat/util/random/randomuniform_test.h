#ifndef TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_TEST_H
#define TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/almostequal.h"
#include <iomanip> // setprecision

inline void TempLat::RandomUniformTester::Test(TempLat::TDDAssertion &tdd)
{
  RandomUniform<> prng("Hello CosmoLattice world!");

  double x = 0;
#ifdef NOKOKKOS
  for (int i = 0; i < 1000000; ++i) {
    x += prng();
  }
  tdd.verify(prng.getState() == 1000000);
  tdd.verify(AlmostEqual(x, 499665.90377910772804170846939087));
#else
  Kokkos::parallel_reduce("RandomUniformTester", 100000, KOKKOS_LAMBDA(int, double &sum) { sum += prng(); }, x);
  tdd.verify(AlmostEqual(x, 50104.227805841801455244421958923));
#endif
  std::cout << "Obtained " << std::setprecision(32) << x << std::endl;
}

#endif
