#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/tdd/tdd.h"
// #include "TempLat/lattice/algebra/constants/positiveinfinitytype.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  namespace Operators
  {
    /** \brief A class which implements the Dirac delta function.
     *
     * Unit test: make test-diracdeltafunction
     **/
    template <typename R> class DiracDeltaFunction : public UnaryOperator<R>
    {
    public:
      using UnaryOperator<R>::mR;

      KOKKOS_FUNCTION
      DiracDeltaFunction(const R &a) : UnaryOperator<R>(a) {}

      /** \brief Getter for two instances. */
      template <typename... IDX>
        requires requires(IDX... idx) { GetValue::get(mR, idx...); }
      KOKKOS_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
      {
        typedef typename GetGetReturnType<R>::type mType;
        mType objValue = GetValue::get(mR, idx...);
        bool isZero = objValue == mType(0);
        return isZero ? std::numeric_limits<mType>::infinity() : mType(0);
      }

      /** \brief Does anyone need derivatives of the delta function? If so, go ahead and figure it out. */
      template <typename U> void d(const U &other) = delete;
      //            {
      //                return GetDeriv::get(mInstanceT, other) * DiracDelta(mInstanceT);
      //            }

      virtual std::string operatorString() const override { return "DiracDelta"; }
    };
  } // namespace Operators

  /** \brief A mini struct for instiating the test case. */
  struct DiracDeltaFunctionTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  /** \brief Exposing our newly define multiplication operation to the world. */
  template <typename T> KOKKOS_FORCEINLINE_FUNCTION Operators::DiracDeltaFunction<T> DiracDelta(const T &a)
  {
    return Operators::DiracDeltaFunction<T>(a);
  }
} // namespace TempLat
#endif
