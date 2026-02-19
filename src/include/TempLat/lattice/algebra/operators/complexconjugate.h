#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_COMPLEXCONJUGATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  /** @brief Enable use of this operator without prefixing std:: or TempLat::.
   * The compiler can distinguish between them. */
  using device::conj;

  namespace Operators
  {
    /** @brief A class which applies complex conjugation to a complex number.
     *
     * Unit test: ctest -R test-multiply
     **/
    template <typename R> class ComplexConjugate : public UnaryOperator<R>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<R>::mR;

      DEVICE_FUNCTION
      ComplexConjugate(const R &a) : UnaryOperator<R>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return conj(DoEval::eval(mR, idx...));
      }

      /** @brief Complex conjugation and copmlex differentiation aren't friends. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other) = delete;
    };
  } // namespace Operators

  /** @brief Exposing our newly define multiplication operation to the world.
   *  Excluded for complex field types (HasComplexFieldGet) which have their own conj overload. */
  template <typename T>
    requires(ConditionalUnaryGetter<T> && !HasComplexFieldGet<T>)
  DEVICE_FORCEINLINE_FUNCTION auto conj(const T &a)
  {
    return Operators::ComplexConjugate<T>(a);
  }
} // namespace TempLat

#endif
