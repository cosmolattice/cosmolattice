#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief Enable use of this operator without prefixing std:: or TempLat::.
   * The compiler can distinguish between them. */
  using device::conj;

  namespace Operators
  {
    /** \brief A class which applies complex conjugation to a complex number.
     * Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template <typename R> class ComplexConjugate : public UnaryOperator<R>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<R>::mR;

      DEVICE_FUNCTION
      ComplexConjugate(const R &a) : UnaryOperator<R>(a) {}

      /** \brief Getter for two instances. */
      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return conj(GetValue::get(mR, idx...));
      }

      /** \brief Complex conjugation and copmlex differentiation aren't friends. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other) = delete;
    };
  } // namespace Operators

  /** \brief A mini struct for instiating the test case. */
  struct ComplexConjugateTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  /** \brief Exposing our newly define multiplication operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  DEVICE_FORCEINLINE_FUNCTION auto conj(const T &a)
  {
    return Operators::ComplexConjugate<T>(a);
  }
} // namespace TempLat

#endif
