#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LOG_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/divide.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  /** @brief Enable use of this operator without prefixing std:: or TempLat::. The compiler can distinguish between
   * them. */
  using device::log;

  /** @brief Extra namespace, as names such as Add and Subtract are too generic. */
  namespace Operators
  {
    /** @brief Get the logarithm of a given expression.
     *
     * Unit test: ctest -R test-log
     **/
    template <typename T> class Log : public UnaryOperator<T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<T>::mR;

      DEVICE_FUNCTION
      Log(const T &a) : UnaryOperator<T>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return log(DoEval::eval(mR, idx...));
      }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        /* not using pow for 1/mInstanceT because pow imports us, log.h */
        return GetDeriv::get(mR, other) / mR;
      }

      virtual std::string operatorString() const override { return "log"; }
    };
  } // namespace Operators

  /** @brief Exposing our newly define log operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  DEVICE_FORCEINLINE_FUNCTION auto log(T a)
  {
    return Operators::Log<T>(a);
  }

  /** @brief Specialize for possible zero output! */
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType log(OneType a) { return {}; }
} // namespace TempLat

#endif
