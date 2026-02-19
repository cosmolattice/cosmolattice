#ifndef TEMPLAT_PARALLEL_STD_STD_H
#define TEMPLAT_PARALLEL_STD_STD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/log/puttostream.h"

#include <ostream>
#include <sys/types.h>

// Including this here, as we need that anywhere basically.
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"

#include <array>
#include <tuple>

#define DEVICE_FUNCTION
#define DEVICE_FORCEINLINE_FUNCTION inline
#define DEVICE_INLINE_FUNCTION inline
#define DEVICE_LAMBDA [&]
#define DEVICE_CLASS_LAMBDA [&, this ]

namespace TempLat
{

  // Need to forward-declare this - we cannot include the layout header here, as it would create a circular
  // dependency.
  template <size_t NDim> struct LayoutStruct;

  namespace std_device
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
    constexpr bool reverse_access_pattern = false;
#endif

    // ------------------------------------------------
    // Standard library replacements in std_device namespace
    // ------------------------------------------------

    template <typename... T> using tuple = std::tuple<T...>;
    template <typename T, std::size_t N> using array = std::array<T, N>;
    using std::apply;
    using std::forward_as_tuple;
    using std::get;
    using std::index_sequence;
    using std::make_tuple;
    using std::tie;
    using std::tuple_cat;

    using Idx = int64_t;
    template <size_t NDim> using IdxArray = std::array<Idx, NDim>;

    // ------------------------------------------------
    // View types
    // ------------------------------------------------

    template <size_t NDim, typename T> class NDView
    {
    };

    template <size_t NDim, typename T> class NDViewUnmanaged
    {
    };
  } // namespace std_device

} // namespace TempLat

/** @brief This one is not related to our algebra, but somehow not
 *  by default enabled in C++ standard library.
 */
template <typename T, typename S>
  requires(std::is_same<T, decltype((T)std::declval<S>())>::value &&
           !(
               requires(std::decay_t<S> s, std::size_t i) { s.eval(i); } ||
               requires(std::decay_t<S> s, std::size_t i, std::size_t j) { s.eval(i, j); } ||
               requires(std::decay_t<S> s, std::size_t i, std::size_t j, std::size_t k) { s.eval(i, j, k); }))
complex<T> operator*(complex<T> a, S b)
{
  return a * (T)b;
}

/** @brief This one is not related to our algebra, but somehow not
 *  by default enabled in C++ standard library.
 */
template <typename T, typename S>
  requires(std::is_same<T, decltype((T)std::declval<S>())>::value &&
           !(
               requires(std::decay_t<S> s, std::size_t i) { s.eval(i); } ||
               requires(std::decay_t<S> s, std::size_t i, std::size_t j) { s.eval(i, j); } ||
               requires(std::decay_t<S> s, std::size_t i, std::size_t j, std::size_t k) { s.eval(i, j, k); }))
DEVICE_FORCEINLINE_FUNCTION complex<T> operator*(S b, complex<T> a)
{
  return a * (T)b;
}

#endif