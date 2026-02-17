#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ABSOLUTEVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ABSOLUTEVALUE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

// imperative to include this:
// otherwise abs might be defined for integers only, with possibly desastrous consequences.

#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/heavisidestepfunction.h"

namespace TempLat
{
  /** @brief Enable use of this operator without prefixing std:: or TempLat::.
   * The compiler can distinguish between them. */
  using device::abs;

  namespace Operators
  {
    /** @brief A class which takes the absolute value of a given expression.
     *
     * Unit test: ctest -R test-multiply
     **/
    template <typename R> class AbsoluteValue : public UnaryOperator<R>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<R>::mR;

      DEVICE_FUNCTION
      AbsoluteValue(const R &a) : UnaryOperator<R>(a) {}

      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return abs(GetValue::get(mR, idx...));
      }

      template <typename... IDX>
        requires IsVariadicIndex<IDX...>
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return abs(DoEval::eval(mR, idx...));
      }

      virtual std::string operatorString() const override { return "abs"; }

      /** @brief Passing on the automatic / symbolic derivatives. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mR, other) * (-heaviside(-mR) + heaviside(mR));
      }
    };
  } // namespace Operators

  /** @brief Exposing our newly defined absolute value operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  DEVICE_FORCEINLINE_FUNCTION auto abs(const T &a)
  {
    return Operators::AbsoluteValue<T>(a);
  }

#ifdef TEMPLATTEST
  /** @brief A mini struct for instiating the test case. */
  struct AbsoluteValueTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
