#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"

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
    DEVICE_FORCEINLINE_FUNCTION static void eval(U &&obj, const IDX &...idx)
    {
      obj.eval(idx...);
    }

    template <typename U, typename... IDX>
      requires(!HasEval<U, IDX...>)
    DEVICE_FORCEINLINE_FUNCTION static constexpr void eval(U &&obj, const IDX &...i)
    {
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    DoEval() = delete;

#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
