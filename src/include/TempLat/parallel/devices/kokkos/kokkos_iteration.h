#ifndef TEMPLAT_PARALLEL_KOKKOS_ITERATION_H
#define TEMPLAT_PARALLEL_KOKKOS_ITERATION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/devices/kokkos/kokkos.h"

namespace TempLat::device_kokkos::iteration
{
      // ===== FOREACH =====
      // 1. foreach: device_kokkos::array
      template <size_t NDim, typename Functor, typename I = device_kokkos::Idx>
        requires requires(Functor functor) { functor(device_kokkos::IdxArray<NDim>{}); }
      void foreach (const std::string &name, const device_kokkos::array<I, NDim> &starts,
                    const device_kokkos::array<I, NDim> &stops, const Functor &functor)
      {
        Kokkos::parallel_for(name, getLocalKokkosPolicy(starts, stops),
                             device_kokkos::KokkosNDLambdaWrapper<NDim, Functor>(functor));
      }
      // 2. foreach: LayoutStruct
      template <size_t NDim, typename Functor>
        requires requires(Functor functor) { functor(device_kokkos::IdxArray<NDim>{}); }
      void foreach (const std::string &name, const LayoutStruct<NDim> &mLayout, const Functor &functor)
      {
        Kokkos::parallel_for(name, device_kokkos::getLocalKokkosPolicy(mLayout),
                             device_kokkos::KokkosNDLambdaWrapper<NDim, Functor>(functor));
      }

      // ===== REDUCE =====
      // 1. reduce: device_kokkos::array -> value
      template <size_t NDim, typename Functor, typename T, typename I = device_kokkos::Idx>
        requires requires(Functor functor, T &update) { functor(device_kokkos::IdxArray<NDim>{}, update); }
      void reduce(const std::string &name, const device_kokkos::array<I, NDim> &starts,
                  const device_kokkos::array<I, NDim> &stops, const Functor &functor, T &result)
      {
        Kokkos::parallel_reduce(name, getLocalKokkosPolicy(starts, stops),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), result);
      }
      // 2. reduce: LayoutStruct -> value
      template <size_t NDim, typename Functor, typename T>
        requires requires(Functor functor, T &update) { functor(device_kokkos::IdxArray<NDim>{}, update); }
      void reduce(const std::string &name, const LayoutStruct<NDim> &mLayout, const Functor &functor, T &result)
      {
        Kokkos::parallel_reduce(name, device_kokkos::getLocalKokkosPolicy(mLayout),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), result);
      }
      // 4. reduce: device_kokkos::array -> View or Reduction
      template <size_t NDim, typename Functor, typename View, typename I = device_kokkos::Idx>
        requires requires(Functor functor, typename View::value_type &update) {
          functor(device_kokkos::IdxArray<NDim>{}, update);
        }
      void reduce(const std::string &name, const device_kokkos::array<I, NDim> &starts,
                  const device_kokkos::array<I, NDim> &stops, const Functor &functor, View view)
      {
        Kokkos::parallel_reduce(name, getLocalKokkosPolicy(starts, stops),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), view);
      }
      // 5. reduce: LayoutStruct -> View or Reduction
      template <size_t NDim, typename Functor, typename View>
        requires requires(Functor functor, typename View::value_type &update) {
          functor(device_kokkos::IdxArray<NDim>{}, update);
        }
      void reduce(const std::string &name, const LayoutStruct<NDim> &mLayout, const Functor &functor, View view)
      {
        Kokkos::parallel_reduce(name, device_kokkos::getLocalKokkosPolicy(mLayout),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), view);
      }

      // ===== REDUCERS =====
      using Kokkos::Max;
      using Kokkos::Min;
      using Kokkos::Prod;
      using Kokkos::Sum;

      // ===== FENCE =====
      inline void fence() { Kokkos::fence(); }
} // namespace TempLat::device_kokkos::iteration

#endif
