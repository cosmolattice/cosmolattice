#ifndef TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_H
#define TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  /** @brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** @brief A class which applies a minus sign.
     * Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: ctest -R test-multiply
     **/
    template <typename T> class UnaryMinus : public UnaryOperator<T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<T>::mR;

      DEVICE_FUNCTION
      UnaryMinus(const T &a) : UnaryOperator<T>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return -DoEval::eval(mR, idx...);
      }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other) { return -GetDeriv::get(mR, other); }

      virtual std::string operatorString() const override { return "-"; }
    };
  } // namespace Operators

  /** @brief Exposing our newly defined subtraction operation to the world. */
  template <typename T>
    requires HasEvalMethod<T>
  DEVICE_FORCEINLINE_FUNCTION auto operator-(const T &a)
  {
    return Operators::UnaryMinus<T>(a);
  }

  /** @brief Specialize for possible zero input! */
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType operator-(ZeroType a) { return a; }

  /** @brief Specialize for double minus signs. */
  template <typename T> DEVICE_FORCEINLINE_FUNCTION auto operator-(Operators::UnaryMinus<Operators::UnaryMinus<T>> &&a)
  {
    return std::move(a);
  }
} // namespace TempLat

#endif
