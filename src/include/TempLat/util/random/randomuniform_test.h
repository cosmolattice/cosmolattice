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
  {
    constexpr size_t N = 1000000;
    RandomUniform prng("Hello CosmoLattice world!");

    double x = 0;

    // Let's draw two generations
    Kokkos::parallel_reduce(
        "RandomUniformTester", N, DEVICE_LAMBDA(int i, double &sum) { sum += prng.get(i, i, 0); }, x);
    tdd.verify(AlmostEqual(x, 500102.3901097552152350545));
    Kokkos::parallel_reduce(
        "RandomUniformTester", N, DEVICE_LAMBDA(int i, double &sum) { sum += prng.get(i, i, 1); }, x);
    tdd.verify(AlmostEqual(x, 499372.8769075584132224321));

    // And let's do that again:
    Kokkos::parallel_reduce(
        "RandomUniformTester", N, DEVICE_LAMBDA(int i, double &sum) { sum += prng.get(i, i, 0); }, x);
    tdd.verify(AlmostEqual(x, 500102.3901097552152350545));
    Kokkos::parallel_reduce(
        "RandomUniformTester", N, DEVICE_LAMBDA(int i, double &sum) { sum += prng.get(i, i, 1); }, x);
    tdd.verify(AlmostEqual(x, 499372.8769075584132224321));

    // Just a brief check, that all generated numbers are different
    Kokkos::View<double *> a("a", 10);
    Kokkos::parallel_for(Kokkos::RangePolicy(0, 10), DEVICE_LAMBDA(const size_t i) { a(i) = prng.get(i, i, 0); });
    auto a_host = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(a_host, a);
    for (size_t i = 0; i < 9; ++i)
      tdd.verify(!AlmostEqual(a_host(i), a_host(i + 1)));

    // If we use the same state, all values should be identical
    Kokkos::View<double *> b("b", 2);
    Kokkos::parallel_for(Kokkos::RangePolicy(0, 2), DEVICE_LAMBDA(const size_t i) { b(i) = prng.get(0, 0, 0); });
    auto b_host = Kokkos::create_mirror_view(b);
    Kokkos::deep_copy(b_host, b);
    tdd.verify(AlmostEqual(b_host(0), b_host(1)));
  }
  {
    // Test saveState/loadState round-trip
    RandomUniform<> rng("serialization_test");
    std::string savedState = rng.saveState();

    // Generate 1000 values after saving state
    std::vector<double> seq1;
    for (int i = 0; i < 1000; ++i) {
      seq1.push_back(rng.get(i, 0, 0));
    }

    // Restore state and generate again
    rng.loadState(savedState);
    std::vector<double> seq2;
    for (int i = 0; i < 1000; ++i) {
      seq2.push_back(rng.get(i, 0, 0));
    }

    // Verify sequences are identical
    bool sequencesMatch = true;
    for (int i = 0; i < 1000; ++i) {
      if (seq1[i] != seq2[i]) {
        sequencesMatch = false;
        break;
      }
    }
    tdd.verify(sequencesMatch, "saveState/loadState round-trip produces identical sequence");
  }
}

#endif
