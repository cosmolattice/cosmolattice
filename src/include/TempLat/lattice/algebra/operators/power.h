#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_POWER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cmath>

#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/helpers/isstdgettable.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/operators/add.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/lattice/algebra/operators/subtract.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/powr.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between
   * them. */
#ifndef NOKOKKOS
  using Kokkos::pow;
#else
  using std::pow;
#endif

  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which takes one getter to the power of another getter. Holds the expression, only evaluates for a
     *single element when you call Power::get(pIterCoords).
     *
     * Unit test: make test-power
     **/
    template <typename R, typename T> class Power : public TempLat::BinaryOperator<R, T>
    {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      KOKKOS_FUNCTION
      Power(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      template <typename... IDX>
        requires requires(IDX... idx) {
          GetValue::get(mR, idx...);
          GetValue::get(mT, idx...);
        }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return pow(GetValue::get(mR, idx...), GetValue::get(mT, idx...));
      }

      static std::string operatorString() { return "^"; }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> KOKKOS_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        using namespace std;
        /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
        return GetDeriv::get(mR, other) * pow(mR, mT - OneType()) + GetDeriv::get(mT, other) * (*this) * log(mT);
      }
    };

    // This class is to make sure std::pow get a static integer. Might be a more elegant way to do it!
    template <ptrdiff_t N, typename R> class PowerN : public UnaryOperator<R>
    {
    public:
      using UnaryOperator<R>::mR;

      KOKKOS_FORCEINLINE_FUNCTION
      PowerN(const R &pR) : UnaryOperator<R>(pR) {}

      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return powr<N>(GetValue::get(mR, idx...));
      }

      std::string toString() const { return "(" + GetString::get(mR) + ")^" + std::to_string(2); }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> KOKKOS_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        using namespace std;
        /* so the compiler chooses without problems between std::log and TempLat::Operators::log */
        return Tag<N>() * PowerN<N - 1, R>(mR) * GetDeriv::get(mR, other);
      }
    };
  } // namespace Operators

  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  KOKKOS_FORCEINLINE_FUNCTION auto pow(const R &r, const T &t)
  {
    return Operators::Power<R, T>(r, t);
  }

  template <ptrdiff_t N> KOKKOS_FORCEINLINE_FUNCTION ZeroType pow(ZeroType) { return ZeroType(); }

  template <typename T> KOKKOS_FORCEINLINE_FUNCTION OneType pow(const T &a, ZeroType b) { return OneType(); }

  /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  template <typename T>
    requires std::is_same_v<T, ZeroType>
  KOKKOS_FORCEINLINE_FUNCTION auto pow(ZeroType a, const T &b)
  {
    return ZeroType();
  }

  // enable if is just so that we can overload to consitently write pow<3>(4)  for std::pow(4,3);
  template <ptrdiff_t N, typename R>
    requires(HasGetMethod<R> && N != 1 && N != 0)
  KOKKOS_FORCEINLINE_FUNCTION auto pow(const R &r)
  {
    return Operators::PowerN<N, R>(r);
  }

  // overload so that we can sonsitently write pow<3>(4)  for std::pow(4,3);
  template <ptrdiff_t N, typename R>
    requires(!HasGetMethod<R> && N != 0 && N != 1 && !(IsTempLatGettable<0, R> || IsSTDGettable<0, R>))
  KOKKOS_FORCEINLINE_FUNCTION auto pow(const R &r)
  {
    return powr<N>(r);
  }

  /** \brief Specialize for possible zero input! */
  template <ptrdiff_t N, typename T>
    requires(N == 0)
  constexpr KOKKOS_FORCEINLINE_FUNCTION auto pow(const T &a)
  {
    return OneType();
  }

  /** \brief Specialize for possible one input! */
  template <ptrdiff_t N, typename T>
    requires(N == 1)
  KOKKOS_FORCEINLINE_FUNCTION T pow(const T &a)
  {
    return a;
  }

  /** \brief A mini struct for instiating the test case. */
  struct PowerTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
