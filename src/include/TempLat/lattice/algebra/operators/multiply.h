#ifndef TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_H
#define TEMPLAT_LATTICE_ALGEBRA_MULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <Kokkos_Macros.hpp>
#include <type_traits>

#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/helpers/isarithmetic.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"

namespace TempLat
{
#ifdef NOKOKKOS
  /** \brief This one is not related to our algebra, but somehow not
   *  by default enabled in C++ standard library.
   */
  template <typename T, typename S>
  typename std::enable_if<std::is_same<T, decltype((T)std::declval<S>())>::value && !HasGetMethod<S>::value,
                          complex<T>>::type
  operator*(complex<T> a, S b)
  {
    return a * (T)b;
  }

  /** \brief This one is not related to our algebra, but somehow not
   *  by default enabled in C++ standard library.
   */
  template <typename T, typename S>
  KOKKOS_FORCEINLINE_FUNCTION
      typename std::enable_if<std::is_same<T, decltype((T)std::declval<S>())>::value && !HasGetMethod<S>::value,
                              complex<T>>::type
      operator*(S b, complex<T> a)
  {
    return a * (T)b;
  }
#endif

  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which multiplies two getters. Holds the expression, only evaluates for a single element when you
     *call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template <typename R, typename T> class Multiplication : public BinaryOperator<R, T>
    {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      KOKKOS_FUNCTION
      Multiplication(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      KOKKOS_FUNCTION
      Multiplication() : BinaryOperator<R, T>(R(), T()) {}

      KOKKOS_FUNCTION
      ~Multiplication() = default;

      template <typename... IDX>
        requires requires(IDX... idx) {
          GetValue::get(mT, idx...);
          GetValue::get(mR, idx...);
        }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return GetValue::get(mT, idx...) * GetValue::get(mR, idx...);
      }

      static std::string operatorString() { return "*"; }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> KOKKOS_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mT, other) * mR + mT * GetDeriv::get(mR, other);
      }
    };

    template <typename R, int N> class MultiplicationN : public UnaryOperator<R>
    {
    public:
      using UnaryOperator<R>::mR;

      KOKKOS_FUNCTION
      MultiplicationN(const R &pR) : UnaryOperator<R>(pR) {}

      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return N * GetValue::get(mR, idx...);
      }

      static std::string operatorString() { return std::to_string(N) + "*"; }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> KOKKOS_FORCEINLINE_FUNCTION auto d(const U &other) { return N * mR; }
    };
  } // namespace Operators

  /** \brief A mini struct for instiating the test case. */
  struct MultiplyTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  /** \brief Exposing our newly define multiplication operation to the world. */
  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  KOKKOS_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return Operators::Multiplication<R, T>(r, t);
  }

  template <typename R, int N>
    requires(HasGetMethod<R> && !(IsTempLatGettable<0, R> || IsSTDGettable<0, R>))
  KOKKOS_FORCEINLINE_FUNCTION auto operator*(const R &r, Tag<N> n)
  {
    return Operators::MultiplicationN<R, N>(r);
  }

  template <typename R, int N>
    requires(HasGetMethod<R> && !(IsTempLatGettable<0, R> || IsSTDGettable<0, R>))
  KOKKOS_FORCEINLINE_FUNCTION auto operator*(Tag<N> n, const R &r)
  {
    return Operators::MultiplicationN<R, N>(r);
  }

  /** \brief Specialize for possible zero input! */
  template <typename T> KOKKOS_FORCEINLINE_FUNCTION ZeroType operator*(const T &a, ZeroType b) { return b; }

  /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  template <typename T>
  KOKKOS_FORCEINLINE_FUNCTION typename std::enable_if<!std::is_same<T, ZeroType>::value, ZeroType>::type
  operator*(ZeroType a, const T &b)
  {
    return a;
  }

  /** \brief Specialize for possible zero input! Need to disable one of these for two OneTypes as input. */
  /** \brief Specialize for possible unit input! */
  template <typename T>
  KOKKOS_FORCEINLINE_FUNCTION
      typename std::enable_if<!std::is_same<T, OneType>::value && !std::is_same<T, ZeroType>::value, T>::type &
      operator*(T &&a, const OneType b)
  {
    return a;
  }

  /** \brief Specialize for possible unit input! */
  template <typename T>
  KOKKOS_FORCEINLINE_FUNCTION
      typename std::enable_if<!std::is_same<T, OneType>::value && !std::is_same<T, ZeroType>::value, T>::type &
      operator*(const OneType a, T &&b)
  {
    return b;
  }

  /** \brief Specialize for possible unit input! */
  template <typename T>
  KOKKOS_FORCEINLINE_FUNCTION
      typename std::enable_if<!std::is_same<T, OneType>::value && !std::is_same<T, ZeroType>::value, T>::type
      operator*(const OneType &a, const T &b)
  {
    return b;
  }

  /** \brief Specialize for possible unit input! */
  KOKKOS_FORCEINLINE_FUNCTION
  OneType operator*(OneType a, OneType b) { return a; }
} // namespace TempLat

#endif
