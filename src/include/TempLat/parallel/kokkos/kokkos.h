#ifndef TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H
#define TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tdd/tddassertion.h"
#include <ostream>
#include <array>

#ifndef NOKOKKOS

#include <Kokkos_Core.hpp>
#include <Kokkos_Random.hpp>

// ------------------------------------------------
// Getting the View types with stars
// ------------------------------------------------

namespace TempLat
{

  template <size_t NDim, typename T> struct GetKokkosNDStarType {
    using type = typename GetKokkosNDStarType<NDim - 1, T>::type *;
  };
  template <typename T> struct GetKokkosNDStarType<1, T> {
    using type = T *;
  };

  // ------------------------------------------------
  // Getting View types
  // ------------------------------------------------

  template <size_t NDim, typename T, typename ExecutionSpace = Kokkos::DefaultExecutionSpace,
            typename Layout = Kokkos::LayoutRight>
  using KokkosNDView = Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, // Get the star syntax for
                                                                                 // dimensionality recursively with
                                    Layout,        // LayoutRight is most compatible for now, may change in future
                                    ExecutionSpace // Choice between GPU and CPU
                                    >;
  template <size_t NDim, typename T, typename ExecutionSpace = Kokkos::DefaultExecutionSpace,
            typename Layout = Kokkos::LayoutRight>
  using KokkosNDViewUnmanaged =
      Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, // Get the star syntax for dimensionality recursively
                                                                // with a helper
                   Layout,         // LayoutRight is most compatible for now, may change in future
                   ExecutionSpace, // Choice between GPU and CPU
                   Kokkos::MemoryTraits<Kokkos::Unmanaged> // No allocation: Attach to existing memory
                   >;

  // ------------------------------------------------
  // Getting ranges to iterate over
  // ------------------------------------------------
  template <size_t NDim> struct KokkosNDRangeHelper {
    using type = Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>;
  };
  template <> struct KokkosNDRangeHelper<1> {
    using type = Kokkos::RangePolicy<>;
  };
  template <size_t NDim> using KokkosNDRange = KokkosNDRangeHelper<NDim>::type;

#else

#define KOKKOS_FORCEINLINE_FUNCTION inline
#define KOKKOS_FUNCTION

#endif

#ifdef TEMPLATTEST
  class KokkosTest
  {
  public:
    KokkosTest() {}
    template <typename TestObjectUnknownHere> static inline void Test(TestObjectUnknownHere &tdd);
  };
#endif
} // namespace TempLat

#endif // KOKKOS_H
