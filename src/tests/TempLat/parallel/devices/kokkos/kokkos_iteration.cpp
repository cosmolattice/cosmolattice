/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/devices/kokkos/kokkos_iteration.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/powr.h"

namespace TempLat
{

  template <size_t NDim> struct KokkosIterationTester {
    static void Test(TDDAssertion &tdd);
  };

  template <size_t NDim> void KokkosIterationTester<NDim>::Test(TDDAssertion &tdd)
  {
    // ------------------------------------------------------------------------------------
    // Testing the KokkosNDLambdaWrapper
    // ------------------------------------------------------------------------------------
    {
      Kokkos::View<double *, Kokkos::DefaultExecutionSpace> a("a", size_t(powr<NDim>(2)));
      auto functor = DEVICE_LAMBDA(const device_kokkos::IdxArray<NDim> &idx)
      {
        // change from "base 2" to base 10
        size_t base10_idx = 0;
        for (size_t i = 0; i < NDim; ++i) {
          // Doing it manually to avoid faulty rounding from pow
          size_t power = 1;
          for (size_t j = 0; j < i; ++j)
            power *= 2;
          base10_idx += idx[i] * power;
        }
        a(base10_idx) = NDim; // just to test that the lambda works
      };

      Kokkos::Array<size_t, NDim> start{};
      Kokkos::Array<size_t, NDim> stop{};
      for (size_t i = 0; i < NDim; ++i)
        stop[i] = 2;

      if constexpr (NDim > 1)
        Kokkos::parallel_for("init", Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start, stop),
                             device_kokkos::KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));
      else
        Kokkos::parallel_for("init", Kokkos::RangePolicy(0, 2),
                             device_kokkos::KokkosNDLambdaWrapper<NDim, decltype(functor)>(functor));
      Kokkos::fence();

      auto host_view = Kokkos::create_mirror_view(a);
      Kokkos::deep_copy(host_view, a);
      bool all_correct = true;
      for (size_t i = 0; i < a.size(); ++i) {
        all_correct &= AlmostEqual(host_view[i], (double)NDim);
        if (!AlmostEqual(host_view[i], (double)NDim)) {
          std::cout << "Error at index " << i << " value is " << host_view[i] << " expected " << (double)NDim << "\n";
        }
      }
      tdd.verify(all_correct);
    }

    // ------------------------------------------------------------------------------------
    // Testing the KokkosNDLambdaWrapperReduction
    // ------------------------------------------------------------------------------------
    {
      auto functor = DEVICE_LAMBDA(const device_kokkos::IdxArray<NDim> &idx, complex<double> &update)
      {
        update += complex<double>((double)NDim, -(double)NDim); // just to test that the lambda works
      };

      Kokkos::Array<size_t, NDim> start{};
      Kokkos::Array<size_t, NDim> stop{};
      for (size_t i = 0; i < NDim; ++i)
        stop[i] = 2;

      complex<double> result{};

      if constexpr (NDim > 1) {
        Kokkos::parallel_reduce("init", Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start, stop),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, decltype(functor)>(functor),
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

      tdd.verify(AlmostEqual(result.real(), (double)NDim * powr<NDim>(2.)) &&
                 AlmostEqual(result.imag(), -(double)NDim * powr<NDim>(2.)));
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::KokkosIterationTester<1>> test_1;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<2>> test_2;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<3>> test_3;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<4>> test_4;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<5>> test_5;
  TempLat::TDDContainer<TempLat::KokkosIterationTester<6>> test_6;
} // namespace
