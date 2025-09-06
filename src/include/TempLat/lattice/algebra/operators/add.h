#ifndef TEMPLAT_LATTICE_ALGEBRA_ADD_H
#define TEMPLAT_LATTICE_ALGEBRA_ADD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/constants/halftype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/lattice/algebra/operators/binaryoperator.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which adds two getters.
     * Holds the expression, only evaluates for a single element when you call Multiply::get(pIterCoords).
     *
     * Unit test: make test-multiply
     **/
    template <typename R, typename T> class Addition : public TempLat::BinaryOperator<R, T>
    {
    public:
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      Addition(const R &pR, const T &pT) : BinaryOperator<R, T>(pR, pT) {}

      template <typename... IDX>
        requires requires(IDX... idx) {
          GetValue::get(mT, idx...);
          GetValue::get(mR, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return TempLat::GetValue::get(mT, idx...) + TempLat::GetValue::get(mR, idx...);
      }

      virtual std::string operatorString() const override { return "+"; }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mT, other) + GetDeriv::get(mR, other);
      }
    };
  } // namespace Operators

  /** \brief A mini struct for instiating the test case. */
  struct AddTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  DEVICE_FORCEINLINE_FUNCTION auto operator+(const R &r, const T &t)
  {
    return Operators::Addition<R, T>(r, t);
  }

  /** \brief Specialize for possible zero input! */
  DEVICE_FORCEINLINE_FUNCTION auto operator+(const ZeroType a, const ZeroType b) { return ZeroType(); }

  /** \brief Specialize for possible half input! */
  DEVICE_FORCEINLINE_FUNCTION
  OneType operator+(const HalfType a, const HalfType b) { return OneType(); }

  /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  template <typename T>
    requires(!std::is_same<T, ZeroType>::value)
  DEVICE_FORCEINLINE_FUNCTION T operator+(const ZeroType a, const T b)
  {
    return b;
  }
  /** \brief Specialize for possible zero input! Need to disable one of these for two ZeroTypes as input. */
  template <typename T>
    requires(!std::is_same<T, ZeroType>::value)
  DEVICE_FORCEINLINE_FUNCTION T operator+(const T b, const ZeroType a)
  {
    return b;
  }
} // namespace TempLat

#endif
