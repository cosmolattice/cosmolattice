//
// Created by franz on 6/4/25.
//

#ifndef TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H
#define TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tdd/tddassertion.h"

#ifndef NOKOKKOS

#include <Kokkos_Core.hpp>

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
