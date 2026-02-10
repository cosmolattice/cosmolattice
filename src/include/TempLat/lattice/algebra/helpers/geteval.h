#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/hasgeteval.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** @brief A class which chooses between get and getEval, to allow for some intermediate caching
   * (useful for operations that derive from matrix multiplication for example).
   *
   *
   * Unit test: ctest -R test-geteval
   **/
  class GetEval
  {
  public:
    // Put public methods here. These should change very little over time.

    template <typename U, typename... IDX>
      requires HasGetEval<U, IDX...>
    static DEVICE_FORCEINLINE_FUNCTION auto getEval(U &&obj, const IDX &...idx)
    {
      // static_assert(std::is_same_v<decltype(obj.getEval(i)), decltype(GetValue::get(obj, i))> && false,
      //               "The return type of getEval must be the same as the return type of get.");
      return obj.getEval(idx...);
    }

    template <typename T> auto extract()
    {
      static_assert(std::is_same_v<double, T> && false,
                    "The return type of getEval must be the same as the return type of get.");
    }

    template <typename U, typename... IDX>
      requires(!HasGetEval<U, IDX...>)
    static DEVICE_FORCEINLINE_FUNCTION auto getEval(U &&obj, const IDX &...idx)
    {
      return GetValue::get(obj, idx...);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    GetEval() {}

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
