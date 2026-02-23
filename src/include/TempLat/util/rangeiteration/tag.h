#ifndef TEMPLAT_UTIL_RANGEITERATION_TAG_H
#define TEMPLAT_UTIL_RANGEITERATION_TAG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"

namespace TempLat
{
  /** @brief A class which implements homemade compile time constants.
   *
   * Unit test: ctest -R test-tag
   **/
  template <int N> class Tag
  {
  public:
    static constexpr int value = N;
    constexpr operator int() const { return N; }

    consteval Tag() = default;

    static std::string toString() { return "Tag<" + std::to_string(N) + ">"; }
  };

  template <int M, int N> Tag<M + N> operator+(Tag<M> t1, Tag<N> t2) { return Tag<M + N>(); }

  template <int M, int N> Tag<M - N> operator-(Tag<M> t1, Tag<N> t2) { return Tag<M - N>(); }

  template <int M> Tag<-M> operator-(Tag<M> t1) { return Tag<-M>(); }

  template <int M, int N> Tag<M * N> operator*(Tag<M> t1, Tag<N> t2) { return Tag<M * N>(); }

  template <int M, int N> constexpr bool operator==(Tag<M> t1, Tag<N> t2) { return M == N; }
  template <int M, int N> constexpr bool operator!=(Tag<M> t1, Tag<N> t2)
  {
    return !std::is_same<Tag<M>, Tag<N>>::value;
  }

  template <int M, int N>
    requires(M < N)
  constexpr bool operator<(Tag<M> t1, Tag<N> t2)
  {
    return true;
  }
  template <int M, int N>
    requires(M >= N)
  constexpr bool operator<(Tag<M> t1, Tag<N> t2)
  {
    return false;
  }

  template <int M, int N>
    requires(M <= N)
  constexpr bool operator<=(Tag<M> t1, Tag<N> t2)
  {
    return true;
  }
  template <int M, int N>
    requires(M > N)
  constexpr bool operator<=(Tag<M> t1, Tag<N> t2)
  {
    return false;
  }

  template <int M, int N>
    requires(M > N)
  constexpr bool operator>(Tag<M> t1, Tag<N> t2)
  {
    return true;
  }
  template <int M, int N>
    requires(M <= N)
  constexpr bool operator>(Tag<M> t1, Tag<N> t2)
  {
    return false;
  }

  template <int M, int N>
    requires(M >= N)
  constexpr bool operator>=(Tag<M> t1, Tag<N> t2)
  {
    return true;
  }
  template <int M, int N>
    requires(M < N)
  constexpr bool operator>=(Tag<M> t1, Tag<N> t2)
  {
    return false;
  }

  template <int N> std::ostream &operator<<(std::ostream &os, const Tag<N> &t)
  {
    os << t.toString();
    return os;
  }

  consteval bool IsMore(auto i, auto j) { return decltype(i)::value > decltype(j)::value; }
  consteval bool IsLess(auto i, auto j) { return decltype(i)::value < decltype(j)::value; }
  consteval bool IsLessOrEqual(auto i, auto j) { return decltype(i)::value <= decltype(j)::value; }
  consteval bool IsMoreOrEqual(auto i, auto j) { return decltype(i)::value >= decltype(j)::value; }
  consteval bool IsEqual(auto i, auto j) { return std::is_same_v<decltype(i), decltype(j)>; }
} // namespace TempLat

#endif
