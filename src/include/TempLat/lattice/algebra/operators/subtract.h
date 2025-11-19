#ifndef TEMPLAT_LATTICE_ALGEBRA_SUBTRACT_H
#define TEMPLAT_LATTICE_ALGEBRA_SUBTRACT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/constants/halftype.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/unaryminus.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/isarithmetic.h"

namespace TempLat
{
  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which subtracts two getters.
     * Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template <typename R, typename T> class Subtraction : public BinaryOperator<R, T>
    {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      DEVICE_FUNCTION
      Subtraction(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      template <typename... IDX>
        requires requires(IDX... idx) {
          GetValue::get(mR, idx...);
          GetValue::get(mT, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return GetValue::get(mR, idx...) - GetValue::get(mT, idx...);
      }

      virtual std::string operatorString() const override { return "-"; }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mR, other) - GetDeriv::get(mT, other);
      }
    };
  } // namespace Operators

  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  DEVICE_FORCEINLINE_FUNCTION Operators::Subtraction<R, T> operator-(const R &r, const T &t)
  {
    return Operators::Subtraction<R, T>(r, t);
  }

  /** \brief Specialize for possible zero input! */
  template <typename T> DEVICE_FORCEINLINE_FUNCTION T &operator-(T &&a, ZeroType b) { return a; }

  /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  //    template <typename T, typename S>
  //    inline
  //    typename std::enable_if<HasGetMethod<T>::value && std::is_same<T, S>::value, ZeroType>::type
  //    operator-( T&& a, T&& b) {
  //        return ZeroType();
  //    }

  /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  template <typename T>
    requires(!std::is_same_v<T, ZeroType>)
  DEVICE_FORCEINLINE_FUNCTION auto operator-(ZeroType a, const T &b)
  {
    return Operators::UnaryMinus<T>(b);
  }

  /** \brief Specialize for unary minus. */
  template <typename T, typename S> DEVICE_FORCEINLINE_FUNCTION auto operator-(T &&a, Operators::UnaryMinus<S> &&b)
  {
    return a + (-b); /* let the double-unary-minus detection take care of peeling b out if it */
  }

  /** \brief Specialize for possible half input! */
  DEVICE_FORCEINLINE_FUNCTION
  HalfType operator-(const OneType a, const HalfType b) { return b; }

  /** \brief Specialize for possible half input! */
  DEVICE_FORCEINLINE_FUNCTION
  auto operator-(HalfType a, OneType b) { return Operators::UnaryMinus<HalfType>(a); }

  /** \brief Specialize for possible OneType OneType input */
  inline auto operator-(OneType a, OneType b) { return ZeroType(); }

  /** \brief A mini struct for instiating the test case. */
  struct SubtractTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
