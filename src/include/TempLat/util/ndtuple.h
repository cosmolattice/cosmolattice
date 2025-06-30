#pragma once

#include <tuple>

namespace TempLat
{
  template <typename T, std::size_t... n> auto n_tuple_impl(std::index_sequence<n...>)
  {
    return std::make_tuple((n, T{})...);
  }

  template <size_t n, typename I> using NDTuple = decltype(n_tuple_impl<I>(std::make_index_sequence<n>()));
} // namespace TempLat