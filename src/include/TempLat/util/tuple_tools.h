#ifndef TEMPLAT_UTIL_TUPLETOOLS_H
#define TEMPLAT_UTIL_TUPLETOOLS_H

#include <tuple>
#include <functional>

#include "TempLat/parallel/device.h"

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
    requires(i <= sizeof...(Tail))
  DEVICE_FORCEINLINE_FUNCTION constexpr auto tuple_last(const device::tuple<Head, Tail...> &t)
  {
    static_assert(i <= sizeof...(Tail), "Cannot take a longer tail than the tuple.");
    if constexpr (sizeof...(Tail) + 1 == i)
      return device::apply([](auto &head, auto &...tail) { return device::tie(head, tail...); }, t);
    else if constexpr (sizeof...(Tail) == i)
      return device::apply([](auto & /*head*/, auto &...tail) { return device::tie(tail...); }, t);
    else
      return device::apply([](auto & /*head*/, auto &...tail) { return tuple_last<i>(device::tie(tail...)); }, t);
  }

  /**
   * @brief Returns the first i elements of the given tuple t
   *
   * @tparam i size of the tuple to be returned
   * @param t tuple to be split
   * @return auto a tied tuple of the first i elements
   */
  template <int i, typename Head, typename... Tail>
  DEVICE_FORCEINLINE_FUNCTION constexpr auto tuple_first(const device::tuple<Head, Tail...> &t)
  {
    static_assert(i <= sizeof...(Tail), "Cannot take a longer sequence than the tuple.");
    static_assert(i >= 0, "Cannot take a longer sequence than the tuple.");
    if constexpr (i == 0)
      return device::tuple();
    else if constexpr (i == 1)
      return device::apply([](auto &head, auto &.../*tail*/) { return device::tie(head); }, t);
    else
      return device::apply(
          [](auto &head, auto &...tail) {
            return device::tuple_cat(device::tie(head), tuple_first<i - 1>(device::tie(tail...)));
          },
          t);
  }

  /**
   * @brief Pass a tuple of indices and add the second argument to the n-th value of the tuple.
   *
   * @tparam n which index of the tuple is to be changed
   * @tparam add the value to add
   * @tparam IDX types of the tuple elements
   * @param tt the tuple to modify
   * @return auto the modified tuple
   */
  template <size_t n, int add, typename... IDX>
  DEVICE_FORCEINLINE_FUNCTION constexpr auto tuple_add_to_nth_mod(device::tuple<IDX...> &tt)
  {
    static_assert(n < sizeof...(IDX));
    device::get<n>(tt) += add;
    return tt;
  }

  template <size_t n, int add, typename... IDX>
  DEVICE_FORCEINLINE_FUNCTION constexpr auto tuple_add_to_nth(const device::tuple<IDX...> &tt)
  {
    constexpr int len = sizeof...(IDX);
    if constexpr (n >= 1) {
      return device::tuple_cat(tuple_first<n>(tt), device::make_tuple((int)device::get<n>(tt) + add),
                               tuple_last<len - n - 1>(tt));
    } else if constexpr (n == 0) {
      return device::tuple_cat(device::make_tuple((int)device::get<0>(tt) + add), tuple_last<len - 1>(tt));
    }
  }

  template <typename... Args, std::size_t... Is>
  DEVICE_FORCEINLINE_FUNCTION auto reverse_tuple(const device::tuple<Args...> &tuple, std::index_sequence<Is...>)
  {
    return device::tie(device::get<sizeof...(Args) - 1 - Is>(tuple)...);
  }

  template <typename... Args> DEVICE_FORCEINLINE_FUNCTION auto reverse_tuple(const device::tuple<Args...> &tuple)
  {
    return reverse_tuple(tuple, std::make_index_sequence<sizeof...(Args)>());
  }

  template <typename Arg, size_t N, std::size_t... Is>
  DEVICE_FORCEINLINE_FUNCTION auto reverse_array(const device::array<Arg, N> &array, std::index_sequence<Is...>)
  {
    return device::array<Arg, N>{{device::get<N - 1 - Is>(array)...}};
  }

  template <typename Arg, size_t N> DEVICE_FORCEINLINE_FUNCTION auto reverse_array(const device::array<Arg, N> &array)
  {
    return reverse_array(array, std::make_index_sequence<N>());
  }

  template <typename T, T... I> constexpr auto make_tuple_sequence_helper(std::integer_sequence<T, I...>)
  {
    return device::make_tuple(I...);
  }

  template <std::size_t I, typename T = std::size_t> constexpr auto make_tuple_sequence()
  {
    return make_tuple_sequence_helper(std::make_integer_sequence<T, I>());
  }
} // namespace TempLat

#endif