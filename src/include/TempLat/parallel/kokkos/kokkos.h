#ifndef TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H
#define TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/tdd/tddassertion.h"
#include "TempLat/util/log/puttostream.h"

#include <ostream>
#include <sys/types.h>

// Including this here, as we need that anywhere basically, where Kokkos is explicitly used.
#include "TempLat/lattice/algebra/helpers/variadicindex.h"

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

    // What's going on here: on GPU, it is beneficial to reverse the memory access pattern, for coalesced access.
    // However, we do not want to impose this on the level of the memory layouts. In particular, this would
    // require additional transpositions when going to Fourier space, which is not what we want. So we do the
    // transposition within the thread dispatch, if we are on a GPU. Otherwise, for optimal cached memory access
    // on CPU, we do not reverse the access pattern.
#if defined(KOKKOS_ENABLE_CUDA) || defined(KOKKOS_ENABLE_HIP) || defined(KOKKOS_ENABLE_SYCL)
    constexpr bool reverse = true;
#else
    constexpr bool reverse = false;
#endif

    for (int d = 0; d < (int)NDim; ++d) {
      const int _d = reverse ? (int)NDim - 1 - d : d;
      start_iteration[_d] = nGhosts;
      stop_iteration[_d] = start_iteration[_d] + localSizes[d];
    }

    if constexpr (NDim == 1) {
      return Kokkos::RangePolicy(start_iteration[0], stop_iteration[0]);
    } else {
      return Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start_iteration, stop_iteration);
    }
  }

  template <size_t NDim> auto getLocalKokkosPolicy(const std::array<ptrdiff_t, NDim> &localSizes)
  {
    Kokkos::Array<uint64_t, NDim> start_iteration;
    Kokkos::Array<uint64_t, NDim> stop_iteration;

    // What's going on here: on GPU, it is beneficial to reverse the memory access pattern, for coalesced access.
    // However, we do not want to impose this on the level of the memory layouts. In particular, this would
    // require additional transpositions when going to Fourier space, which is not what we want. So we do the
    // transposition within the thread dispatch, if we are on a GPU. Otherwise, for optimal cached memory access
    // on CPU, we do not reverse the access pattern.
#if defined(KOKKOS_ENABLE_CUDA) || defined(KOKKOS_ENABLE_HIP) || defined(KOKKOS_ENABLE_SYCL)
    constexpr bool reverse = true;
#else
    constexpr bool reverse = false;
#endif

    for (int d = 0; d < (int)NDim; ++d) {
      const int _d = reverse ? (int)NDim - 1 - d : d;
      start_iteration[_d] = 0;
      stop_iteration[_d] = start_iteration[_d] + localSizes[d];
    }

    if constexpr (NDim == 1) {
      return Kokkos::RangePolicy(start_iteration[0], stop_iteration[0]);
    } else {
      return Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(start_iteration, stop_iteration);
    }
  }

  template <size_t NDim>
  auto getLocalKokkosPolicy(const LayoutStruct<NDim> &layout, Kokkos::DefaultExecutionSpace &exec)
  {
    Kokkos::Array<uint64_t, NDim> start_iteration;
    Kokkos::Array<uint64_t, NDim> stop_iteration;
    const auto localSizes = layout.getLocalSizes();
    const size_t nGhosts = layout.getNGhosts();

    // What's going on here: on GPU, it is beneficial to reverse the memory access pattern, for coalesced access.
    // However, we do not want to impose this on the level of the memory layouts. In particular, this would
    // require additional transpositions when going to Fourier space, which is not what we want. So we do the
    // transposition within the thread dispatch, if we are on a GPU. Otherwise, for optimal cached memory access
    // on CPU, we do not reverse the access pattern.
#if defined(KOKKOS_ENABLE_CUDA) || defined(KOKKOS_ENABLE_HIP) || defined(KOKKOS_ENABLE_SYCL)
    constexpr bool reverse = true;
#else
    constexpr bool reverse = false;
#endif

    for (int d = 0; d < (int)NDim; ++d) {
      const int _d = reverse ? (int)NDim - 1 - d : d;
      start_iteration[_d] = nGhosts;
      stop_iteration[_d] = start_iteration[_d] + localSizes[d];
    }

    if constexpr (NDim == 1) {
      return Kokkos::RangePolicy(exec, start_iteration[0], stop_iteration[0]);
    } else {
      return Kokkos::MDRangePolicy<Kokkos::Rank<NDim>>(exec, start_iteration, stop_iteration);
    }
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

    using Idx = uint64_t;
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

  using Idx = uint64_t;
  template <size_t NDim> using IdxArray = std::array<Idx, NDim>;
#endif
  } // namespace device

  template <typename T, size_t N>
    requires(!std::is_same_v<std::array<T, N>, device::array<T, N>>)
  std::ostream &operator<<(std::ostream &stream, const device::array<T, N> &vec)
  {
    return PutToStream(stream, vec);
  };

  template <typename T, size_t N> std::ostream &operator<<(std::ostream &stream, const Kokkos::Array<T, N> &vec)
  {
    return PutToStream(stream, vec);
  };

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
