#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** @brief A class providing a function which automatically calls DoEval::eval before GetValue::get. Only for
   * convenience, but should not be used in anything but assignments/readouts on the single-field level (i.e. ConfigView
   * and FourierView).
   *
   * Unit test: ctest -R test-geteval
   **/
  class GetEval
  {
  public:
    // Put public methods here. These should change very little over time.

    /**
     * @brief Calls DoEval::eval on the object, and then returns the value via GetValue::get.
     *
     * @param obj The object to evaluate and get the value from.
     * @param idx The indices to get the value at.
     * @return The value of the object at the given indices, after evaluating it.
     */
    template <typename U, typename... IDX> static DEVICE_FORCEINLINE_FUNCTION auto getEval(U &&obj, const IDX &...idx)
    {
      DoEval::eval(obj);
      return GetValue::get(obj, idx...);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    GetEval() {}

#ifdef TEMPLATTEST
  public:
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
