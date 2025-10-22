#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/cosine.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  using device::sin;

  /** \brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** \brief A class which applies cosine.
     *
     * Unit test: make test-multiply
     **/
    template <typename T> class Sine : public UnaryOperator<T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<T>::mR;

      DEVICE_FUNCTION
      Sine(const T &a) : UnaryOperator<T>(a) {}

      /** \brief Getter for two instances. */
      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        return sin(GetValue::get(mR, idx...));
      }

      /** \brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mR, other) * cos(mR);
      }

      virtual std::string operatorString() const override { return "sin"; }
    };
  } // namespace Operators

  /** \brief A mini struct for instiating the test case. */
  struct SineTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  /** \brief Exposing our newly define exp operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  DEVICE_FORCEINLINE_FUNCTION auto sin(T a)
  {
    return Operators::Sine<T>(a);
  }

  /** \brief Specialize for possible zero input! */
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType sin(ZeroType a) { return ZeroType(); }
} // namespace TempLat

#endif
