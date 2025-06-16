#ifndef TEMPLAT_UTIL_MAKEUNIFORMARRAY_H
#define TEMPLAT_UTIL_MAKEUNIFORMARRAY_H

#include <array>
#include <cstddef>

namespace TempLat
{
  template <typename T, size_t N> inline std::array<T, N> makeUniformArray(const T &value)
  {
    auto ret = std::array<T, N>{};
    for (size_t i = 0; i < N; ++i)
      ret[i] = value;
    return ret;
  }
} // namespace TempLat

#endif