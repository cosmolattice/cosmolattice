#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVALUE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/iscomplextype.h"
#include "TempLat/parallel/device.h"

namespace TempLat
{
  template <typename U, typename... IDX>
  concept TypeHasGet = requires(std::decay_t<U> obj, IDX... i) { obj.get(i...); };

  template <typename U, typename... IDX>
  concept TypeGetsItself = (std::is_arithmetic_v<std::decay_t<U>> || IsComplexType<std::decay_t<U>>);

  /** \brief A template-programming class which helps to get the 'get'-value from any type,
   * whether it has the get-method or it is a scalar value.
   * Now you can use an int as if it had a get method for example.
   *
   * Unit test: make test-getvalue
   **/
  class GetValue
  {
  public:
    // Put public methods here. These should change very little over time.

    template <typename U, typename... IDX>
      requires TypeHasGet<U, IDX...>
    static DEVICE_FORCEINLINE_FUNCTION auto get(U &&obj, const IDX &...idx)
    {
      return obj.get(idx...);
    }

    template <typename U, typename... IDX>
      requires TypeGetsItself<U, IDX...>
    static DEVICE_FORCEINLINE_FUNCTION auto get(U &&obj, const IDX &...idx)
    {
      return obj;
    }

    template <typename U, typename... IDX>
      requires requires {
        requires !TypeHasGet<U, IDX...>;
        requires !TypeGetsItself<U, IDX...>;
        requires std::is_arithmetic_v<std::decay_t<decltype(U::value)>>;
      }
    static DEVICE_FORCEINLINE_FUNCTION auto get(const U &obj, const IDX &...idx)
    {
      return std::decay_t<U>::value;
    }

    template <typename U> static auto get_example(U &&obj)
    {
      if constexpr (TypeHasGet<U, int>) {
        return get(obj, 0);
      } else if constexpr (TypeHasGet<U, int, int>) {
        return get(obj, 0, 0);
      } else if constexpr (TypeHasGet<U, int, int, int>) {
        return get(obj, 0, 0, 0);
      } else if constexpr (TypeHasGet<U, int, int, int, int>) {
        return get(obj, 0, 0, 0, 0);
      } else if constexpr (TypeHasGet<U, int, int, int, int, int>) {
        return get(obj, 0, 0, 0, 0, 0);
      } else if constexpr (TypeHasGet<U, int, int, int, int, int, int>) {
        return get(obj, 0, 0, 0, 0, 0, 0);
      } else if constexpr (TypeHasGet<U, int, int, int, int, int, int, int>) {
        return get(obj, 0, 0, 0, 0, 0, 0, 0);
      } else
        return obj;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    GetValue() = delete;

  public:
#ifdef TEMPLATTEST
    template <typename T> static inline void Test(T &tdd);
#endif
  };
} // namespace TempLat

#endif
