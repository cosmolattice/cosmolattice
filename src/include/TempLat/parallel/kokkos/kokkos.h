#ifndef TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H
#define TEMPLAT_PARALLEL_KOKKOS_KOKKOS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/log/puttostream.h"

#include <Kokkos_Core.hpp>
#include <Kokkos_Complex.hpp>

#ifdef KOKKOS_ENABLE_CUDA
#include <cuda/std/array>
#include <cuda/std/tuple>
#else
#include <array>
#include <tuple>
#endif

#define DEVICE_FUNCTION KOKKOS_FUNCTION
#define DEVICE_FORCEINLINE_FUNCTION KOKKOS_FORCEINLINE_FUNCTION
#define DEVICE_INLINE_FUNCTION KOKKOS_INLINE_FUNCTION
#define DEVICE_LAMBDA KOKKOS_LAMBDA
#define DEVICE_CLASS_LAMBDA KOKKOS_CLASS_LAMBDA

namespace TempLat
{
  namespace device_kokkos
  {
    // What's going on here: on GPU, it is beneficial to reverse the memory access pattern, for coalesced access.
    // However, we do not want to impose this on the level of the memory layouts. In particular, this would
    // require additional transpositions when going to Fourier space, which is not what we want. So we do the
    // transposition within the thread dispatch, if we are on a GPU. Otherwise, for optimal cached memory access
    // on CPU, we do not reverse the access pattern.
#ifdef FORCE_ACCESS_PATTERN

#if FORCE_ACCESS_PATTERN == 0
    constexpr bool reverse_access_pattern = false;
#elif FORCE_ACCESS_PATTERN == 1
    constexpr bool reverse_access_pattern = true;
#endif

#else

#if defined(KOKKOS_ENABLE_CUDA) || defined(KOKKOS_ENABLE_HIP) || defined(KOKKOS_ENABLE_SYCL)
    constexpr bool reverse_access_pattern = true;
#else
    constexpr bool reverse_access_pattern = false;
#endif

#endif

    // ------------------------------------------------
    // Standard library replacements in device_kokkos namespace
    // ------------------------------------------------

#ifdef KOKKOS_ENABLE_CUDA
    template <typename T, std::size_t N> using array = cuda::std::array<T, N>;
    using cuda::std::apply;
    using cuda::std::forward_as_tuple;
    using cuda::std::get;
    using cuda::std::index_sequence;
    using cuda::std::make_index_sequence;
    using cuda::std::make_tuple;
    using cuda::std::tie;
    using cuda::std::tuple;
    using cuda::std::tuple_cat;

    using Idx = uint64_t;
    template <size_t NDim> using IdxArray = cuda::std::array<Idx, NDim>;
#else
    template <typename T, std::size_t N> using array = std::array<T, N>;
    using std::apply;
    using std::forward_as_tuple;
    using std::get;
    using std::index_sequence;
    using std::make_index_sequence;
    using std::make_tuple;
    using std::tie;
    using std::tuple;
    using std::tuple_cat;

    using Idx = uint64_t;
    template <size_t NDim> using IdxArray = std::array<Idx, NDim>;
#endif

    // ------------------------------------------------
    // Arithmetic defaults in device_kokkos namespace
    // ------------------------------------------------

    using Kokkos::abs;
    using Kokkos::acos;
    using Kokkos::acosh;
    using Kokkos::asin;
    using Kokkos::asinh;
    using Kokkos::atan;
    using Kokkos::atan2;
    using Kokkos::atanh;
    using Kokkos::ceil;
    using Kokkos::conj;
    using Kokkos::cos;
    using Kokkos::cosh;
    using Kokkos::exp;
    using Kokkos::floor;
    using Kokkos::fmod;
    using Kokkos::imag;
    using Kokkos::log;
    using Kokkos::max;
    using Kokkos::min;
    using Kokkos::pow;
    using Kokkos::real;
    using Kokkos::sin;
    using Kokkos::sinh;
    using Kokkos::sqrt;
    using Kokkos::tan;
    using Kokkos::tanh;

    template <typename T> using complex = Kokkos::complex<T>;
  } // namespace device_kokkos

  template <typename T, size_t N>
    requires(!std::is_same_v<std::array<T, N>, device_kokkos::array<T, N>>)
  std::ostream &operator<<(std::ostream &stream, const device_kokkos::array<T, N> &vec)
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

#include "TempLat/parallel/kokkos/kokkos_internal.h"

#endif // KOKKOS_H
