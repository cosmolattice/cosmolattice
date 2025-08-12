#ifndef TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_H
#define TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_H

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
    KOKKOS_FORCEINLINE_FUNCTION void operator()(Args &&...args) const
    {
      fun({{std::forward<Args>(args)...}});
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
   * @tparam RED The type of the reduction result
   */
  template <size_t NDim, typename FUN, typename RED> struct KokkosNDLambdaWrapperReduction {
    KokkosNDLambdaWrapperReduction(const FUN &_fun) : fun(_fun) {};

    template <typename... Args>
      requires(sizeof...(Args) == NDim + 1)
    KOKKOS_FORCEINLINE_FUNCTION void operator()(Args &&...args) const
    {
      auto tuple = device::tie(std::forward<Args>(args)...);
      fun(makeArray(tuple_first<NDim>(tuple)), device::get<NDim>(tuple)); // the last argument is the reduction result
    }

    FUN fun;

    template <typename... Args>
      requires(sizeof...(Args) == NDim)
    KOKKOS_FORCEINLINE_FUNCTION auto makeArray(device::tuple<Args...> &&tuple) const
    {
      return device::apply([](auto &&...args) { return device::IdxArray<NDim>{{args...}}; }, tuple);
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