#ifndef TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_TEST_H
#define TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include <cstdint>
#include <cuda.h>

template <size_t NDim> void TempLat::KokkosNDLambdaWrapperTester<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  Kokkos::View<double *, Kokkos::DefaultExecutionSpace> a("a", size_t(pow(2, NDim)));
  auto functor = KOKKOS_LAMBDA(const std::array<size_t, NDim> &idx)
  {
    // change from "base 2" to base 10
    size_t base10_idx = 0;
    for (size_t i = 0; i < NDim; ++i) {
      base10_idx += idx[i] * static_cast<size_t>(pow(2, i));
    }
    a(base10_idx) = NDim; // just to test that the lambda works
  };

  Kokkos::Array<int64_t, NDim> start{};
  Kokkos::Array<int64_t, NDim> stop{};
  for (size_t i = 0; i < NDim; ++i)
    stop[i] = 2;

  if constexpr (NDim > 1)
    Kokkos::parallel_for("init", Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start, stop),
                         KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));
  else
    Kokkos::parallel_for("init", Kokkos::RangePolicy(0, 2), KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));
  Kokkos::fence();

  auto host_view = Kokkos::create_mirror_view(a);
  Kokkos::deep_copy(host_view, a);
  bool all_correct = true;
  for (size_t i = 0; i < a.size(); ++i)
    all_correct &= TempLat::AlmostEqual(host_view[i], (double)NDim);
  tdd.verify(all_correct);
}

#endif
