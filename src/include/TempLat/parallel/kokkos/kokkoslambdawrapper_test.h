#ifndef TEMPLAT_PARALLEL_DEVICE_LAMBDAWRAPPER_TEST_H
#define TEMPLAT_PARALLEL_DEVICE_LAMBDAWRAPPER_TEST_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/powr.h"

template <size_t NDim> void TempLat::KokkosNDLambdaWrapperTester<NDim>::Test(TempLat::TDDAssertion &tdd)
{
  // ------------------------------------------------------------------------------------
  // Testing the KokkosNDLambdaWrapper
  // ------------------------------------------------------------------------------------
  {
    Kokkos::View<double *, Kokkos::DefaultExecutionSpace> a("a", size_t(powr<NDim>(2)));
    auto functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx)
    {
      // change from "base 2" to base 10
      size_t base10_idx = 0;
      for (size_t i = 0; i < NDim; ++i) {
        base10_idx += idx[i] * static_cast<size_t>(pow(2, i));
      }
      a(base10_idx) = NDim; // just to test that the lambda works
    };

    device::IdxArray<NDim> start{};
    device::IdxArray<NDim> stop{};
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

  // ------------------------------------------------------------------------------------
  // Testing the KokkosNDLambdaWrapperReduction
  // ------------------------------------------------------------------------------------
  {
    auto functor = DEVICE_LAMBDA(const device::IdxArray<NDim> &idx, complex<double> &update)
    {
      update += complex<double>((double)NDim, -(double)NDim); // just to test that the lambda works
    };

    device::IdxArray<NDim> start{};
    device::IdxArray<NDim> stop{};
    for (size_t i = 0; i < NDim; ++i)
      stop[i] = 2;

    complex<double> result{};

    if constexpr (NDim > 1) {
      Kokkos::parallel_reduce("init", Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start, stop),
                              KokkosNDLambdaWrapperReduction<NDim, decltype(functor), complex<double>>(functor),
                              result);
    } else {
      Kokkos::parallel_reduce(
          "init", Kokkos::RangePolicy(0, 2),
          DEVICE_LAMBDA(const size_t idx, complex<double> &update) {
            update += complex<double>((double)NDim, -(double)NDim); // just to test that the lambda works
          },
          result);
    }
    Kokkos::fence();

    tdd.verify(TempLat::AlmostEqual(result.real(), (double)NDim * powr<NDim>(2.)) &&
               TempLat::AlmostEqual(result.imag(), -(double)NDim * powr<NDim>(2.)));
  }
}

#endif
