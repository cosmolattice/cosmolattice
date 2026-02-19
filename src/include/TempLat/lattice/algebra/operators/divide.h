#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIVIDE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIVIDE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/subtract.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"

namespace TempLat
{
  /** @brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** @brief A class which divides two expressions. Holds the expression, only evaluates for a single element when you
     *call Divide::get(pIterCoords).
     *
     * Unit test: ctest -R test-divide
     **/
    template <typename R, typename T> class Division : public BinaryOperator<R, T>
    {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      DEVICE_FUNCTION
      Division(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return DoEval::eval(mR, idx...) / DoEval::eval(mT, idx...);
      }

      virtual std::string operatorString() const override { return "/"; }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        /* not using pow for mT * mT, because pow imports log which imports us, divide.h */
        return GetDeriv::get(mR, other) / mT - GetDeriv::get(mT, other) * mR / (mT * mT);
      }
    };

    /** @brief Check  if numerator if roughly zero, don't do the division.
     * Useful for spectrum fluctuation, when normalising with a cutoff
     */
    template <typename R, typename T> struct SafeDivision : public BinaryOperator<R, T> {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      DEVICE_FUNCTION
      SafeDivision(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        const auto a = DoEval::eval(mR, idx...);
        const auto b = DoEval::eval(mT, idx...);

        constexpr decltype(a / b) zero{};

        return AlmostEqual(a, zero) ? zero : a / b;
      }

      virtual std::string operatorString() const override { return "/safe/"; }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        /* not using pow for mT * mT, because pow imports log which imports us, divide.h */
        return GetDeriv::get(mR, other) / mT - GetDeriv::get(mT, other) * mR / (mT * mT);
      }
    };
  } // namespace Operators

  /** @brief Exposing our newly define multiplication operation to the world. */
  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  DEVICE_FORCEINLINE_FUNCTION auto operator/(const R &r, const T &t)
  {
    return Operators::Division<R, T>(r, t);
  }

  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  DEVICE_FORCEINLINE_FUNCTION auto safeDivide(const R &r, const T &t)
  {
    return Operators::SafeDivision<R, T>(r, t);
  }

  /** @brief Specialize for possible unit input! Simplify derivatives for example. */
  template <typename T> DEVICE_FORCEINLINE_FUNCTION T operator/(const T &a, OneType b) { return a; }

  /** @brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  template <typename T>
    requires std::is_same_v<T, ZeroType>
  DEVICE_FORCEINLINE_FUNCTION auto operator/(const ZeroType &a, const T &b)
  {
    return a;
  }
} // namespace TempLat

#endif
