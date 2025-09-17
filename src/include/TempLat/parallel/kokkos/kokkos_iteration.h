#ifndef TEMPLAT_PARALLEL_KOKKOS_ITERATION_H
#define TEMPLAT_PARALLEL_KOKKOS_ITERATION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/kokkos/kokkos.h"

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  namespace device_kokkos
  {
    namespace iteration
    {
      template <size_t NDim, typename Functor, typename I>
        requires requires(Functor functor) { functor(device_kokkos::IdxArray<NDim>{}); }
      void parallel_for(const std::string &name, const device_kokkos::array<I, NDim> &starts,
                        const device_kokkos::array<I, NDim> &stops, const Functor &functor)
      {
        Kokkos::parallel_for(name, getLocalKokkosPolicy(starts, stops),
                             device_kokkos::KokkosNDLambdaWrapper<NDim, Functor>(functor));
      }
      template <size_t NDim, typename Functor, typename I>
        requires requires(Functor functor) {
          functor(device_kokkos::IdxArray<NDim>{});
          requires !std::same_as<std::array<I, NDim>, device_kokkos::array<I, NDim>>;
        }
      void parallel_for(const std::string &name, const std::array<I, NDim> &starts, const std::array<I, NDim> &stops,
                        const Functor &functor)
      {
        Kokkos::parallel_for(name, getLocalKokkosPolicy(starts, stops),
                             device_kokkos::KokkosNDLambdaWrapper<NDim, Functor>(functor));
      }

      template <size_t NDim, typename Functor>
        requires requires(Functor functor) { functor(device_kokkos::IdxArray<NDim>{}); }
      void parallel_for(const std::string &name, const LayoutStruct<NDim> &mLayout, const Functor &functor)
      {
        Kokkos::parallel_for(name, device_kokkos::getLocalKokkosPolicy(mLayout),
                             device_kokkos::KokkosNDLambdaWrapper<NDim, Functor>(functor));
      }

      template <size_t NDim, typename Functor, typename I, typename T>
        requires requires(Functor functor, T &update) { functor(device_kokkos::IdxArray<NDim>{}, update); }
      void parallel_reduce(const std::string &name, const device_kokkos::array<I, NDim> &starts,
                           const device_kokkos::array<I, NDim> &stops, const Functor &functor, T &result)
      {
        Kokkos::parallel_reduce(name, getLocalKokkosPolicy(starts, stops),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), result);
      }
      template <size_t NDim, typename Functor, typename I, typename T>
        requires requires(Functor functor, T &update) {
          functor(device_kokkos::IdxArray<NDim>{}, update);
          requires !std::same_as<std::array<I, NDim>, device_kokkos::array<I, NDim>>;
        }
      void parallel_reduce(const std::string &name, const std::array<I, NDim> &starts, const std::array<I, NDim> &stops,
                           const Functor &functor, T &result)
      {
        Kokkos::parallel_reduce(name, getLocalKokkosPolicy(starts, stops),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), result);
      }

      template <size_t NDim, typename Functor, typename T>
        requires requires(Functor functor, T &update) { functor(device_kokkos::IdxArray<NDim>{}, update); }
      void parallel_reduce(const std::string &name, const LayoutStruct<NDim> &mLayout, const Functor &functor,
                           T &result)
      {
        Kokkos::parallel_reduce(name, device_kokkos::getLocalKokkosPolicy(mLayout),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), result);
      }

      template <size_t NDim, typename Functor, typename View>
        requires requires(Functor functor, typename View::value_type &update) {
          functor(device_kokkos::IdxArray<NDim>{}, update);
        }
      void parallel_reduce(const std::string &name, const LayoutStruct<NDim> &mLayout, const Functor &functor,
                           View view)
      {
        Kokkos::parallel_reduce(name, device_kokkos::getLocalKokkosPolicy(mLayout),
                                device_kokkos::KokkosNDLambdaWrapperReduction<NDim, Functor>(functor), view);
      }

      inline void fence() { Kokkos::fence(); }
    } // namespace iteration
  } // namespace device_kokkos

#ifdef TEMPLATTEST
  template <size_t NDim> class KokkosIterationTester
  {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
