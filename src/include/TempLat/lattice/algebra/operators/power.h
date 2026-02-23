#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/operators/add.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/subtract.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/powr.h"

namespace TempLat
{
  /** @brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between
   * them. */
  using device::pow;

  /** @brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** @brief A class which takes one expression to the power of another expression.
     *
     * Unit test: ctest -R test-power
     **/
    template <typename R, typename T> class Power : public TempLat::BinaryOperator<R, T>
    {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      DEVICE_FUNCTION
      Power(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return pow(DoEval::eval(mR, idx...), DoEval::eval(mT, idx...));
      }

      virtual std::string operatorString() const override { return "^"; }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
        return GetDeriv::get(mR, other) * pow(mR, mT - OneType()) + GetDeriv::get(mT, other) * (*this) * log(mT);
      }
    };

    template <ptrdiff_t N, typename R> class PowerN : public UnaryOperator<R>
    {
    public:
      using UnaryOperator<R>::mR;

      DEVICE_FORCEINLINE_FUNCTION
      PowerN(const R &pR) : UnaryOperator<R>(pR) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return powr<N>(DoEval::eval(mR, idx...));
      }

      std::string toString() const { return "(" + GetString::get(mR) + ")^" + std::to_string(2); }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other) const
      {
        /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
        return Tag<N>() * PowerN<N - 1, R>(mR) * GetDeriv::get(mR, other);
      }
    };
  } // namespace Operators

  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  DEVICE_FORCEINLINE_FUNCTION auto pow(const R &r, const T &t)
  {
    return Operators::Power<R, T>(r, t);
  }

  template <ptrdiff_t N> DEVICE_FORCEINLINE_FUNCTION ZeroType pow(ZeroType) { return {}; }

  template <typename T> DEVICE_FORCEINLINE_FUNCTION OneType pow(const T &a, ZeroType b) { return {}; }

  /** @brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  template <typename T>
    requires std::is_same_v<T, ZeroType>
  DEVICE_FORCEINLINE_FUNCTION auto pow(ZeroType a, const T &b)
  {
    return ZeroType();
  }

  // enable if is just so that we can overload to consitently write pow<3>(4)  for std::pow(4,3);
  template <ptrdiff_t N, typename R>
    requires(HasEvalMethod<R> && N != 1 && N != 0)
  DEVICE_FORCEINLINE_FUNCTION auto pow(const R &r)
  {
    return Operators::PowerN<N, R>(r);
  }

  // overload so that we can sonsitently write pow<3>(4)  for std::pow(4,3);
  template <ptrdiff_t N, typename R>
    requires requires(R r) {
      requires !HasEvalMethod<R>;
      requires N != 0;
      requires N != 1;
      requires !(IsTempLatGettable<0, R> || IsSTDGettable<0, R>);
      powr<N>(r);
    }
  DEVICE_FORCEINLINE_FUNCTION auto pow(const R &r)
  {
    return powr<N>(r);
  }

  /** @brief Specialize for possible zero input! */
  template <ptrdiff_t N, typename T>
    requires(N == 0)
  constexpr DEVICE_FORCEINLINE_FUNCTION auto pow(const T &a)
  {
    return OneType();
  }

  /** @brief Specialize for possible one input! */
  template <ptrdiff_t N, typename T>
    requires(N == 1)
  DEVICE_FORCEINLINE_FUNCTION T pow(const T &a)
  {
    return a;
  }
} // namespace TempLat

#endif
