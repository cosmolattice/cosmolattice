#ifndef TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_H
#define TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isarithmetic.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/getcpptypename.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"

namespace TempLat
{
  /** @brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** @brief A class which multiplies two expressions.
     *
     * Unit test: ctest -R test-multiply
     **/
    template <typename R, typename T> class Multiplication : public BinaryOperator<R, T>
    {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      DEVICE_FUNCTION
      Multiplication(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return DoEval::eval(mR, idx...) * DoEval::eval(mT, idx...);
      }

      virtual std::string operatorString() const override { return "*"; }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mT, other) * mR + mT * GetDeriv::get(mR, other);
      }
    };

    template <typename R, int N> class MultiplicationN : public UnaryOperator<R>
    {
    public:
      using UnaryOperator<R>::mR;

      MultiplicationN(const R &pR) : UnaryOperator<R>(pR) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return N * DoEval::eval(mR, idx...);
      }

      virtual std::string operatorString() const override { return std::to_string(N) + "*"; }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other) { return N * mR; }
    };
  } // namespace Operators

  /** @brief Exposing our newly define multiplication operation to the world. */
  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return Operators::Multiplication<R, T>(r, t);
  }

  template <typename R, int N>
    requires(HasEvalMethod<R> && !(IsTempLatGettable<0, R> || IsSTDGettable<0, R>))
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, Tag<N> n)
  {
    return Operators::MultiplicationN<R, N>(r);
  }

  template <typename R, int N>
    requires(HasEvalMethod<R> && !(IsTempLatGettable<0, R> || IsSTDGettable<0, R>))
  DEVICE_FORCEINLINE_FUNCTION auto operator*(Tag<N> n, const R &r)
  {
    return Operators::MultiplicationN<R, N>(r);
  }

  /** @brief Specialize for possible zero input! */
  template <typename T>
    requires(!std::is_same_v<T, ZeroType>)
  DEVICE_FORCEINLINE_FUNCTION ZeroType operator*(const T &a, ZeroType b)
  {
    return b;
  }
  /** @brief Specialize for possible zero input! */
  template <typename T>
    requires(!std::is_same_v<T, ZeroType>)
  DEVICE_FORCEINLINE_FUNCTION ZeroType operator*(ZeroType a, const T &b)
  {
    return a;
  }

  /** @brief Specialize for possible unit input! */
  template <typename T>
    requires(!std::is_same_v<T, OneType> && !std::is_same_v<T, ZeroType>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const T &a, const OneType b)
  {
    return a;
  }
  /** @brief Specialize for possible unit input! */
  template <typename T>
    requires(!std::is_same_v<T, OneType> && !std::is_same_v<T, ZeroType>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const OneType &a, const T &b)
  {
    return b;
  }

  /** @brief Specialize for possible unit input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType operator*(OneType a, OneType b) { return a; }
} // namespace TempLat

#endif
