#ifndef TEMPLAT_UTIL_TUPLETOOLS_H
#define TEMPLAT_UTIL_TUPLETOOLS_H

#include <tuple>
#include <functional>

#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat
{
  // If you wonder about how performant this is, take a look at https://godbolt.org/z/d33cMaEG1

  /**
   * @brief Returns the last i elements of the given tuple t
   *
   * @tparam i size of the tuple to be returned
   * @param t tuple to be split
   * @return auto a tied tuple of the last i elements
   */
  template <size_t i, typename Head, typename... Tail>
  KOKKOS_FORCEINLINE_FUNCTION constexpr auto tuple_last(const std::tuple<Head, Tail...> &t)
  {
    static_assert(i <= sizeof...(Tail), "Cannot take a longer tail than the tuple.");
    if constexpr (sizeof...(Tail) == i)
      return std::apply([](auto & /*head*/, auto &...tail) { return std::tie(tail...); }, t);
    else
      return std::apply([](auto & /*head*/, auto &...tail) { return tuple_last<i>(std::tie(tail...)); }, t);
  }

  /**
   * @brief Returns the first i elements of the given tuple t
   *
   * @tparam i size of the tuple to be returned
   * @param t tuple to be split
   * @return auto a tied tuple of the first i elements
   */
  template <size_t i, typename Head, typename... Tail>
  KOKKOS_FORCEINLINE_FUNCTION constexpr auto tuple_first(const std::tuple<Head, Tail...> &t)
  {
    static_assert(i <= sizeof...(Tail), "Cannot take a longer sequence than the tuple.");
    if constexpr (i == 0)
      return std::tuple();
    else if constexpr (i == 1)
      return std::apply([](auto &head, auto &.../*tail*/) { return std::tie(head); }, t);
    else
      return std::apply(
          [](auto &head, auto &...tail) {
            return std::tuple_cat(std::tie(head), tuple_first<i - 1>(std::tie(tail...)));
          },
          t);
  }

  /**
   * @brief Pass a tuple of indices and add the second argument to the n-th value of the tuple.
   *
   * @tparam n which index of the tuple is to be changed
   * @tparam I type of the value to add
   * @tparam IDX types of the tuple elements
   * @param tt the tuple to modify
   * @param add the value to add
   * @return auto the modified tuple
   */
  template <size_t n, typename I, typename... IDX>
  KOKKOS_FORCEINLINE_FUNCTION constexpr auto tuple_add_to_nth_mod(std::tuple<IDX...> &&tt, const I &add)
  {
    static_assert(n < sizeof...(IDX));
    std::get<n>(tt) += add;
    return tt;
  }

  template <size_t n, typename I, typename... IDX>
  KOKKOS_FORCEINLINE_FUNCTION constexpr auto tuple_add_to_nth_mod(std::tuple<IDX...> &tt, const I &add)
  {
    static_assert(n < sizeof...(IDX));
    std::get<n>(tt) += add;
    return tt;
  }

  template <size_t n, typename I, typename... IDX>
  KOKKOS_FORCEINLINE_FUNCTION constexpr auto tuple_add_to_nth(const std::tuple<IDX...> &tt, const I &add)
  {
    constexpr size_t len = sizeof...(IDX);
    static_assert(n < len);
    if constexpr (n >= 1) {
      return std::tuple_cat(tuple_first<n>(tt), std::tuple(std::get<n>(tt) + add), tuple_last<len - n - 1>(tt));
    } else if constexpr (n == 0) {
      return std::tuple_cat(std::tuple(std::get<0>(tt) + add), tuple_last<len - 1>(tt));
    }
  }

#ifdef TEMPLATTEST
  class TupleToolsTester
  {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif