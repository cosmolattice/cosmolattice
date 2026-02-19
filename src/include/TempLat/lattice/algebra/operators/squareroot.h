#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SQUAREROOT_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SQUAREROOT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/operators/power.h"

#include "TempLat/lattice/algebra/constants/halftype.h"

namespace TempLat
{
  /** @brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between
   * them. */
  using device::sqrt;

  namespace Operators
  {
    /** @brief A function which applies a square root, by forwarding to power(x, 0.5), with 0.5 templated explicitly.
     * Holds the expression, only evaluates for a single element when you call SquareRoot::get(pIterCoords).
     *
     * Unit test: ctest -R test-multiply
     **/
    template <typename R> struct SafeSqrt : public UnaryOperator<R> {
    public:
      using UnaryOperator<R>::mR;

      DEVICE_FUNCTION
      SafeSqrt(const R &pR) : UnaryOperator<R>(pR) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        const auto a = DoEval::eval(mR, idx...);
        constexpr decltype(a) zero{};
        return (a < zero) ? zero : sqrt(a);
      }

      virtual std::string operatorString() const override { return "safe_sqrt"; }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION void d(const U &other) = delete;
    };
  } // namespace Operators

  template <typename R>
    requires ConditionalUnaryGetter<R>
  DEVICE_FORCEINLINE_FUNCTION auto safeSqrt(const R &r)
  {
    return Operators::SafeSqrt<R>(r);
  }

  template <typename T>
    requires(ConditionalBinaryGetter<T, HalfType> && !std::is_arithmetic_v<T>)
  DEVICE_FORCEINLINE_FUNCTION auto sqrt(T a)
  {
    return Operators::Power<T, HalfType>(a, HalfType());
  }

  /** @brief Specialize for possible zero input! */
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType sqrt(ZeroType a) { return a; }

  /** @brief Specialize for possible unit input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType sqrt(OneType a) { return a; }
} // namespace TempLat

#endif
