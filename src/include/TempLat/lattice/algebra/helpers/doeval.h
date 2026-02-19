#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/haseval.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat
{
  /** @brief A class which helps implementing the intermediate evaluation mechanism,
   * extremely useful for matrix algebra.
   *
   * Unit test: ctest -R test-doeval
   **/
  class DoEval
  {
  public:
    // Put public methods here. These should change very little over time.
    template <typename U, typename... IDX>
      requires HasEval<U, IDX...>
    DEVICE_FORCEINLINE_FUNCTION static auto eval(U &&obj, const IDX &...idx)
    {
      return obj.eval(idx...);
    }

    template <typename U, typename... IDX>
      requires(!HasEval<U, IDX...> && TypeHasStaticValue<U>)
    DEVICE_FORCEINLINE_FUNCTION static constexpr auto eval(U &&obj, const IDX &...i)
    {
      return std::decay_t<U>::value;
    }

    template <typename U, typename... IDX>
      requires TypeEvalsItself<U>
    static DEVICE_FORCEINLINE_FUNCTION auto eval(U &&obj, const IDX &...idx)
    {
      return obj;
    }

    template <typename U> static constexpr auto eval_example(U &&obj)
    {
      if constexpr (HasEval<U, int>)
        return eval(obj, int{});
      else if constexpr (HasEval<U, int, int>)
        return eval(obj, int{}, int{});
      else if constexpr (HasEval<U, int, int, int>)
        return eval(obj, int{}, int{}, int{});
      else if constexpr (HasEval<U, int, int, int, int>)
        return eval(obj, int{}, int{}, int{}, int{});
      else if constexpr (HasEval<U, int, int, int, int, int>)
        return eval(obj, int{}, int{}, int{}, int{}, int{});
      else if constexpr (HasEval<U, int, int, int, int, int, int>)
        return eval(obj, int{}, int{}, int{}, int{}, int{}, int{});
      else if constexpr (HasEval<U, int, int, int, int, int, int, int>)
        return eval(obj, int{}, int{}, int{}, int{}, int{}, int{}, int{});
      else if constexpr (TypeHasStaticValue<U>)
        return std::decay_t<U>::value;
      else
        return obj;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    DoEval() = delete;
  };
} // namespace TempLat

#endif
