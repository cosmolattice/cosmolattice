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
  constexpr size_t N = 1000000;
  if (N < (size_t)Kokkos::DefaultExecutionSpace().concurrency())
    say << "Warning: your concurrency is " << Kokkos::DefaultExecutionSpace().concurrency()
        << " which is greater than N=" << N << ". This should cause the last test to fail.\n";

  RandomUniform prng("Hello CosmoLattice world!", N);

  double x = 0;
#ifdef NOKOKKOS
  for (int i = 0; i < N; ++i) {
    x += prng();
  }
  tdd.verify(prng.getState() == N);
  tdd.verify(AlmostEqual(x, 499665.90377910772804170846939087));
#else
  // Check the reproducibility
  Kokkos::parallel_reduce("RandomUniformTester", N, KOKKOS_LAMBDA(int i, double &sum) { sum += prng(i); }, x);
  tdd.verify(AlmostEqual(x, 499891.06055758631555363535881042));
  Kokkos::parallel_reduce("RandomUniformTester", N, KOKKOS_LAMBDA(int i, double &sum) { sum += prng(i); }, x);
  tdd.verify(AlmostEqual(x, 500006.233279812964610755443573));

  prng.rebase();

  Kokkos::parallel_reduce("RandomUniformTester", N, KOKKOS_LAMBDA(int i, double &sum) { sum += prng(i); }, x);
  tdd.verify(AlmostEqual(x, 499891.06055758631555363535881042));
  Kokkos::parallel_reduce("RandomUniformTester", N, KOKKOS_LAMBDA(int i, double &sum) { sum += prng(i); }, x);
  tdd.verify(AlmostEqual(x, 500006.233279812964610755443573));

  prng.rebase();

  Kokkos::parallel_reduce("RandomUniformTester", N, KOKKOS_LAMBDA(int i, double &sum) { sum += prng(i); }, x);
  tdd.verify(AlmostEqual(x, 499891.06055758631555363535881042));
  Kokkos::parallel_reduce("RandomUniformTester", N, KOKKOS_LAMBDA(int i, double &sum) { sum += prng(i); }, x);
  tdd.verify(AlmostEqual(x, 500006.233279812964610755443573));

  prng.rebase();

  // Just a brief check, that all generated numbers are different
  Kokkos::View<double *> a("a", 10);
  Kokkos::parallel_for(Kokkos::RangePolicy(0, 10), KOKKOS_LAMBDA(const size_t i) { a(i) = prng(i); });
  auto a_host = Kokkos::create_mirror_view(a);
  Kokkos::deep_copy(a_host, a);
  for (size_t i = 0; i < 9; ++i)
    tdd.verify(!AlmostEqual(a_host(i), a_host(i + 1)));

  // If we use the same state, and the size of the parallel region is smaller than the concurrency, all values should be
  // identical
  Kokkos::View<double *> b("b", 2);
  Kokkos::parallel_for(Kokkos::RangePolicy(0, 2), KOKKOS_LAMBDA(const size_t i) { b(i) = prng(0); });
  auto b_host = Kokkos::create_mirror_view(b);
  Kokkos::deep_copy(b_host, b);
  tdd.verify(AlmostEqual(b_host(0), b_host(1)));

  // If we go beyond the concurrency, even identical states should lead to differing values
  Kokkos::View<double *> c("c", N + 4);
  Kokkos::parallel_for(Kokkos::RangePolicy(0, N + 4), KOKKOS_LAMBDA(const size_t i) { c(i) = prng(i % N); });
  auto c_host = Kokkos::create_mirror_view(c);
  Kokkos::deep_copy(c_host, c);
  tdd.verify(!AlmostEqual(c_host(0), c_host(N)));

#endif
}

#endif
