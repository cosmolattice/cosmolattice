#ifndef TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_H
#define TEMPLAT_LATTICE_ALGEBRA_UNARYMINUS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which applies a minus sign.
     * Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template <typename T> class UnaryMinus : public UnaryOperator<T>
    {
    public:
      /* Put public methods here. These should change very little over time. */
      using UnaryOperator<T>::mR;

      KOKKOS_FUNCTION
      UnaryMinus(const T &a) : UnaryOperator<T>(a) {}

      /** \brief Getter for two instances. */
      template <std::integral... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return -GetValue::get(mR, idx...);
      }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> KOKKOS_FORCEINLINE_FUNCTION auto d(const U &other) { return -GetDeriv::get(mR, other); }

      static std::string operatorString() { return "-"; }
    };
  } // namespace Operators

  /** \brief A mini struct for instantiating the test case. */
  struct UnaryMinusTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  /** \brief Exposing our newly defined subtraction operation to the world. */
  template <typename T>
    requires HasGetMethod<T>
  KOKKOS_FORCEINLINE_FUNCTION auto operator-(const T &a)
  {
    return Operators::UnaryMinus<T>(a);
  }

  /** \brief Specialize for possible zero input! */
  KOKKOS_FORCEINLINE_FUNCTION
  ZeroType operator-(ZeroType a) { return a; }

  /** \brief Specialize for double minus signs. */
  template <typename T> KOKKOS_FORCEINLINE_FUNCTION auto operator-(Operators::UnaryMinus<Operators::UnaryMinus<T>> &&a)
  {
    return a;
  }
} // namespace TempLat

#endif
