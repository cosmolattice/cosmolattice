#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_DIRACDELTAFUNCTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
// #include "TempLat/lattice/algebra/constants/positiveinfinitytype.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  namespace Operators
  {
    /** @brief A class which implements the Dirac delta function.
     *
     * Unit test: ctest -R test-diracdeltafunction
     **/
    template <typename R> class DiracDeltaFunction : public UnaryOperator<R>
    {
    public:
      using UnaryOperator<R>::mR;

      DEVICE_FUNCTION
      DiracDeltaFunction(const R &a) : UnaryOperator<R>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        using mType = typename GetGetReturnType<R>::type;
        const mType objValue = DoEval::eval(mR, idx...);
        const bool isZero = objValue == mType{};
        return isZero ? std::numeric_limits<mType>::max() : mType{};
      }

      /** @brief Does anyone need derivatives of the delta function? If so, go ahead and figure it out. */
      template <typename U> void d(const U &other) = delete;
      //            {
      //                return GetDeriv::get(mInstanceT, other) * DiracDelta(mInstanceT);
      //            }

      virtual std::string operatorString() const override { return "DiracDelta"; }
    };
  } // namespace Operators

  /** @brief Exposing our newly define multiplication operation to the world. */
  template <typename T> DEVICE_FORCEINLINE_FUNCTION Operators::DiracDeltaFunction<T> DiracDelta(const T &a)
  {
    return Operators::DiracDeltaFunction<T>(a);
  }
} // namespace TempLat
#endif
