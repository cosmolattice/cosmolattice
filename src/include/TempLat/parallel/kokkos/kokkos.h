#ifndef TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H
#define TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tdd/tddassertion.h"

#include <ostream>
#include <sys/types.h>

// Including this here, as we need that anywhere basically, where Kokkos is explicitly used.
#include "TempLat/lattice/algebra/helpers/variadicindex.h"

#include "TempLat/lattice/algebra/helpers/getvalue.h"

#ifndef NOKOKKOS

#include <Kokkos_Core.hpp>
#include <Kokkos_Random.hpp>

#ifdef KOKKOS_ENABLE_CUDA
#include <cuda/std/array>
#include <cuda/std/tuple>
#else
#include <array>
#include <tuple>
#endif

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

  using DefaultLayout = Kokkos::LayoutRight; // Default layout for Kokkos views, can be changed if needed.

  template <size_t NDim, typename T, typename ExecutionSpace = Kokkos::DefaultExecutionSpace,
            typename Layout = DefaultLayout>
  using KokkosNDView = Kokkos::View<typename GetKokkosNDStarType<NDim, T>::type, // Get the star syntax for
                                                                                 // dimensionality recursively with
                                    Layout,        // LayoutRight is most compatible for now, may change in future
                                    ExecutionSpace // Choice between GPU and CPU
                                    >;
  template <size_t NDim, typename T, typename ExecutionSpace = Kokkos::DefaultExecutionSpace,
            typename Layout = DefaultLayout>
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

  // Need to forward-declare this - we cannot include the layout header here, as it would create a circular dependency.
  template <size_t NDim> struct LayoutStruct;

  template <size_t NDim> auto getLocalKokkosPolicy(const LayoutStruct<NDim> &layout)
  {
    Kokkos::Array<uint64_t, NDim> start_iteration;
    Kokkos::Array<uint64_t, NDim> stop_iteration;
    const auto localSizes = layout.getLocalSizes();
    const size_t nGhosts = layout.getNGhosts();

    for (size_t d = 0; d < NDim; ++d) {
      start_iteration[d] = nGhosts;
      stop_iteration[d] = start_iteration[d] + localSizes[d];
    }
    return KokkosNDRange<NDim>(start_iteration, stop_iteration);
  }

#else

#define KOKKOS_FORCEINLINE_FUNCTION inline
#define KOKKOS_FUNCTION

#endif

  namespace device
  {
#ifdef KOKKOS_ENABLE_CUDA
    template <typename... T> using tuple = cuda::std::tuple<T...>;
    template <typename T, std::size_t N> using array = cuda::std::array<T, N>;
    using cuda::std::apply;
    using cuda::std::forward_as_tuple;
    using cuda::std::get;
    using cuda::std::index_sequence;
    using cuda::std::make_tuple;
    using cuda::std::tie;
    using cuda::std::tuple_cat;

    using Idx = uint32_t;
    template <size_t NDim> using IdxArray = cuda::std::array<Idx, NDim>;
#else
  template <typename... T> using tuple = std::tuple<T...>;
  template <typename T, std::size_t N> using array = std::array<T, N>;
  using std::apply;
  using std::forward_as_tuple;
  using std::get;
  using std::index_sequence;
  using std::make_tuple;
  using std::tie;
  using std::tuple_cat;

  using Idx = uint32_t;
  template <size_t NDim> using IdxArray = std::array<Idx, NDim>;
#endif
  } // namespace device

  template <typename OBJ, size_t NDim, typename T>
  void setAtOnePoint(OBJ &&obj, device::array<ptrdiff_t, NDim> pos, T val)
  {
    Kokkos::parallel_for(
        "Set a point", Kokkos::RangePolicy(0, 1),
        KOKKOS_LAMBDA(const uint) { device::apply([&](const auto... idx) { obj.getSet(idx...) = val; }, pos); });
  }

  template <typename OBJ, size_t NDim>
  GetGetReturnType<OBJ>::type getAtOnePoint(OBJ &&obj, const device::array<ptrdiff_t, NDim> &pos)
  {
    using T = GetGetReturnType<OBJ>::type;
    T ret;

    Kokkos::parallel_reduce(
        "Get a point", Kokkos::RangePolicy(0, 1),
        KOKKOS_LAMBDA(const uint, T &update) {
          device::apply([&](const auto... idx) { update = GetValue::get(obj, idx...); }, pos);
        },
        ret);
    return ret;
  }

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
