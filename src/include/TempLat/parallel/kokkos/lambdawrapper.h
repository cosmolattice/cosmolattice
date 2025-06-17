#ifndef TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_H
#define TEMPLAT_PARALLEL_KOKKOS_LAMBDAWRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat
{
  template <size_t NDim, typename FUN> struct KokkosNDLambdaWrapper {
    KokkosNDLambdaWrapper(const FUN &_fun) : fun(_fun) {};

    template <typename... Args>
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