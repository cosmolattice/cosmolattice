//
// Created by franz on 6/4/25.
//

#ifndef TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H
#define TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tdd/tddassertion.h"

#ifndef NOKOKKOS

#include <Kokkos_Core.hpp>

template <size_t NDim, typename T> struct GetKokkosNDStarType {
  using type = typename GetKokkosNDStarType<NDim - 1, T>::type *;
};

template <typename T> struct GetKokkosNDStarType<1, T> {
  using type = T *;
};

template <size_t NDim, typename T, typename ExecutionSpace = Kokkos::DefaultExecutionSpace>
using KokkosNDView = Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, ExecutionSpace>;

template <size_t NDim, typename T, typename ExecutionSpace = Kokkos::DefaultExecutionSpace>
using KokkosNDViewUnmanaged =
    Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, ExecutionSpace, Kokkos::MemoryTraits<Kokkos::Unmanaged>>;

#else

#define KOKKOS_FORCEINLINE_FUNCTION inline
#define KOKKOS_FUNCTION

#endif

namespace TempLat
{
#ifdef TEMPLATTEST
  class KokkosTest
  {
  public:
    KokkosTest() {}
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/parallel/kokkos/kokkos_test.h"
#endif

#endif // KOKKOS_H
