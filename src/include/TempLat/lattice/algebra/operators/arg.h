#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_ARG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2026

#include "TempLat/lattice/algebra/conditional/conditionalbinarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/util/constants.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"

namespace TempLat
{
  using device::atan2;
  namespace Operators
  {
    /** @brief A class to compute the argument of a complex field.
     *
     * Unit test: ctest -R test-arg
     **/
    template <typename R, typename T> class Arg : public BinaryOperator<R, T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using BinaryOperator<R, T>::mR;
      using BinaryOperator<R, T>::mT;

      DEVICE_FUNCTION
      Arg(R r, T t) : BinaryOperator<R, T>(r, t) {}

      DEVICE_FUNCTION
      Arg() : BinaryOperator<R, T>(R(), T()) {}

      template <typename... IDX>
        requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(r, idx...);
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        const auto res = atan2(DoEval::eval(mR, idx...), DoEval::eval(mT, idx...));
        return AlmostEqual(res, 0) ? 0 : ((res > 0) ? res : res + 2 * Constants::pi<double>);
      }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> void d(const U &other) = delete;

      virtual std::string operatorString() const override { return "arg"; }
    };
  } // namespace Operators

  template <typename R, typename T>
    requires ConditionalBinaryGetter<R, T>
  DEVICE_FORCEINLINE_FUNCTION auto arg(R r, T t)
  {
    return Operators::Arg<R, T>{r, t};
  }
} // namespace TempLat

#endif
