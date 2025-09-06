#ifndef TEMPLAT_PARALLEL_DEVICE_LAMBDAWRAPPER_H
#define TEMPLAT_PARALLEL_DEVICE_LAMBDAWRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/kokkos/kokkos.h"
#include "TempLat/util/tuple_tools.h"

namespace TempLat
{
  /**
   * @brief This is a functor which wraps a lambda.
   * Basically, this is necessary when one wants to call a variadic lambda on an NVIDIA GPU.
   * CUDA seems to be unable to expand the variadic arguments - in contrast, a direct approach does indeed work for
   * openMP or serial compilation.
   * To get around this limitation, the KokkosNDLambdaWrapper packs the indices into an array.
   * If you wonder, whether there's a difference when using tie and tuples: https://godbolt.org/z/M3bG39rsM
   * No. Therefore, we spare the ourselves the hassle and simply use an array.
   *
   * @tparam NDim Number of arguments taken
   * @tparam FUN The lambda to which we forward the indices
   */
  template <size_t NDim, typename FUN> struct KokkosNDLambdaWrapper {
    KokkosNDLambdaWrapper(const FUN &_fun) : fun(_fun) {};

    template <typename... Args>
      requires(sizeof...(Args) == NDim)
    DEVICE_FORCEINLINE_FUNCTION void operator()(const Args &...args) const
    {
      // What's going on here: on GPU, it is beneficial to reverse the memory access pattern, for coalesced access.
      // However, we do not want to impose this on the level of the memory layouts. In particular, this would
      // require additional transpositions when going to Fourier space, which is not what we want. So we do the
      // transposition within the thread dispatch, if we are on a GPU. Otherwise, for optimal cached memory access
      // on CPU, we do not reverse the access pattern.
#if defined(KOKKOS_ENABLE_CUDA) || defined(KOKKOS_ENABLE_HIP) || defined(KOKKOS_ENABLE_SYCL)
      fun(reverse_array(device::IdxArray<NDim>{{args...}}));
#else
      fun({{std::forward<Args>(args)...}});
#endif
    }

    FUN fun;
  };

  /**
   * @brief This is a functor which wraps a lambda for reduction.
   * Basically, this is necessary when one wants to call a variadic lambda on an NVIDIA GPU.
   * CUDA seems to be unable to expand the variadic arguments - in contrast, a direct approach does indeed work for
   * openMP or serial compilation.
   * To get around this limitation, the KokkosNDLambdaWrapperReduction packs the indices into an array.
   * If you wonder, whether there's a difference when using tie and tuples: https://godbolt.org/z/M3bG39rsM
   * No. Therefore, we spare the ourselves the hassle and simply use an array.
   *
   * @tparam NDim Number of arguments taken
   * @tparam FUN The lambda to which we forward the indices
   */
  template <size_t NDim, typename FUN> struct KokkosNDLambdaWrapperReduction {
    KokkosNDLambdaWrapperReduction(const FUN &_fun) : fun(_fun) {};

    template <typename... Args>
      requires(sizeof...(Args) == NDim + 1)
    DEVICE_FORCEINLINE_FUNCTION void operator()(Args &&...args) const
    {
      // What's going on here: on GPU, it is beneficial to reverse the memory access pattern, for coalesced access.
      // However, we do not want to impose this on the level of the memory layouts. In particular, this would
      // require additional transpositions when going to Fourier space, which is not what we want. So we do the
      // transposition within the thread dispatch, if we are on a GPU. Otherwise, for optimal cached memory access
      // on CPU, we do not reverse the access pattern.
      auto tuple = device::tie(args...);
#if defined(KOKKOS_ENABLE_CUDA) || defined(KOKKOS_ENABLE_HIP) || defined(KOKKOS_ENABLE_SYCL)
      fun(makeArray(reverse_tuple(tuple_first<NDim>(tuple))),
          device::get<NDim>(tuple)); // the last argument is the reduction result
#else
      fun(makeArray(tuple_first<NDim>(tuple)), device::get<NDim>(tuple)); // the last argument is the reduction result
#endif
    }

    FUN fun;

    template <typename... Args>
      requires(sizeof...(Args) == NDim)
    DEVICE_FORCEINLINE_FUNCTION auto makeArray(device::tuple<Args...> &&tuple) const
    {
      return device::apply([](const auto &...args) { return device::IdxArray<NDim>{{args...}}; }, tuple);
    }
  };

  template <size_t NDim> class KokkosNDLambdaWrapperTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif