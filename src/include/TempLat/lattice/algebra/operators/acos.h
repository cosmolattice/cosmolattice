#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ACOS_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ACOS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/divide.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

#include "TempLat/lattice/algebra/helpers/getderiv.h"

namespace TempLat
{
  using device::acos;

  /** @brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** @brief A class which applies arccosine.
     *
     * Unit test: ctest -R test-acos
     **/
    template <typename T> class ACos : public UnaryOperator<T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<T>::mR;

      DEVICE_FUNCTION
      ACos(const T &a) : UnaryOperator<T>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return acos(DoEval::eval(mR, idx...));
      }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return -GetDeriv::get(mR, other) * (1.0 / sqrt(1 - pow<2>(mR)));
      }

      virtual std::string operatorString() const override { return "acos"; }
    };
  } // namespace Operators

  /** @brief Exposing our newly define acos operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  DEVICE_FORCEINLINE_FUNCTION auto acos(T a)
  {
    return Operators::ACos<T>(a);
  }

  /** @brief Specialize for possible unit input! acos(1) = 0. */
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType acos(OneType a) { return {}; }
} // namespace TempLat

#endif
