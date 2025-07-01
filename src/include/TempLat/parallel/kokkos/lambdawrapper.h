#ifndef TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_H
#define TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat
{
  /**
   * @brief This is a functor which wraps a lambda.
   * Basically, this is necessary when one wants to call a variadic lambda on an NVIDIA GPU.
   * CUDA seems to be unable to expand the variadic arguments - in contrast, a direct approach does indeed work for
   * openMP or serial compilation.
   * To get around this limitation, the KokkosNDLambdaWrapper packs the indices into an array.
   * If you wonder, whether there's a difference when using std::tie and tuples: https://godbolt.org/z/M3bG39rsM
   * No. Therefore, we spare the ourselves the hassle and simply use std::array.
   *
   * @tparam NDim Number of arguments taken
   * @tparam FUN The lambda to which we forward the indices
   */
  template <size_t NDim, typename FUN> struct KokkosNDLambdaWrapper {
    KokkosNDLambdaWrapper(const FUN &_fun) : fun(_fun) {};

    template <std::integral... Args>
      requires(sizeof...(Args) == NDim)
    KOKKOS_FORCEINLINE_FUNCTION void operator()(Args... args) const
      requires(std::convertible_to<Args, std::common_type_t<Args...>> && ...)
    {
      fun({{args...}});
    }

    FUN fun;
  };

  template <size_t NDim> class KokkosNDLambdaWrapperTester
  {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/parallel/kokkos/lambdawrapper_test.h"
#endif

#endif