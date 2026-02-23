#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_SINE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  using device::sin;

  /** @brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** @brief A class which applies the sine to an expression.
     *
     * Unit test: ctest -R test-sine
     **/
    template <typename T> class Sine : public UnaryOperator<T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<T>::mR;

      DEVICE_FUNCTION
      Sine(const T &a) : UnaryOperator<T>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return sin(DoEval::eval(mR, idx...));
      }

      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mR, other) * cos(mR);
      }

      virtual std::string operatorString() const override { return "sin"; }
    };
  } // namespace Operators

  /** @brief Exposing our newly define exp operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  DEVICE_FORCEINLINE_FUNCTION auto sin(T a)
  {
    return Operators::Sine<T>(a);
  }

  /** @brief Specialize for possible zero input! */
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType sin(ZeroType a) { return {}; }
} // namespace TempLat

#endif
