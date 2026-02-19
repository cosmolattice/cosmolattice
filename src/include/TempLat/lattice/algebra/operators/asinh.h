#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ASINH_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_ASINH_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/helpers/getderiv.h"
#include "TempLat/lattice/algebra/operators/exponential.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"

namespace TempLat
{
  using device::asinh;
  namespace Operators
  {
    /** @brief A class which computes the asinh.
     *
     * Unit test: ctest -R test-asinh
     **/
    template <typename T> class ASinh : public UnaryOperator<T>
    {
    public:
      // Put public methods here. These should change very little over time.
      using UnaryOperator<T>::mR;

      DEVICE_FUNCTION
      ASinh(const T &a) : UnaryOperator<T>(a) {}

      template <typename... IDX>
        requires requires(std::decay_t<T> t, IDX... idx) {
          requires IsVariadicIndex<IDX...>;
          DoEval::eval(t, idx...);
        }
      DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
      {
        return asinh(DoEval::eval(mR, idx...));
      }

      /** @brief And passing on the automatic / symbolic derivatives. Having fun here, this is awesome. */
      template <typename U> DEVICE_FORCEINLINE_FUNCTION auto d(const U &other)
      {
        return 1 / sqrt(1 + (*this) * (*this)) * GetDeriv::get(mR, other);
      }

      virtual std::string operatorString() const override { return "asinh"; }
    };
  } // namespace Operators

  /** @brief Exposing our newly define exp operation to the world. */
  template <typename T>
    requires ConditionalUnaryGetter<T>
  DEVICE_FORCEINLINE_FUNCTION auto asinh(T a)
  {
    return Operators::ASinh<T>(a);
  }
} // namespace TempLat

#endif
