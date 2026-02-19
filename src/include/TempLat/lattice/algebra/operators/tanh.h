#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_TANH_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_TANH_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/sinh.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include <type_traits>

namespace TempLat
{
  using device::tanh;

  namespace Operators
  {
    /** @brief A class which implements the Tanh.
     *
     * Unit test: ctest -R test-tanh
     **/
    template <typename T> class Tanh : public UnaryOperator<T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<T>::mR;

      DEVICE_FUNCTION
      Tanh(const T &a) : UnaryOperator<T>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return tanh(DoEval::eval(mR, idx...));
      }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return GetDeriv::get(mR, other) / pow<2>(sinh(*this));
      }

      virtual std::string operatorString() const override { return "tanh"; }
    };
  } // namespace Operators

  /** @brief Exposing our newly define exp operation to the world. */
  template <typename T>
    requires(!std::is_arithmetic_v<T> && !IsComplexType<T>)
  DEVICE_FORCEINLINE_FUNCTION auto tanh(T a)
  {
    return Operators::Tanh<T>(a);
  }

  /** @brief Specialize for possible zero input! */
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType tanh(ZeroType a) { return ZeroType(); }
} // namespace TempLat

#endif
