#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include <type_traits>

namespace TempLat
{
  /** @brief A class which implements scalar multiplication over complex numbers.
   *
   * Unit test: ctest -R test-scalarcomplexfieldmultiply
   **/

  template <typename R, typename T> class ScalarComplexFieldMultiply : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    DEVICE_FUNCTION
    ScalarComplexFieldMultiply(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return mR * Real(mT); }

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return mR * Imag(mT); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      const auto cmplx = DoEval::eval(mT, idx...);
      const auto scalar = DoEval::eval(mR, idx...);
      device::array<decltype(scalar * cmplx[0]), 2> result;
      result[0] = scalar * cmplx[0];
      result[1] = scalar * cmplx[1];
      return result;
    }

    virtual std::string operatorString() const override { return "*"; }
  };

  template <typename T>
  concept IsScalarType =
      (std::is_arithmetic_v<T> || HasEvalMethod<T>) && !HasComplexFieldGet<T> && !HasSU2Get<T> && !HasSU2DoubletGet<T>;

  template <typename R, typename T>
    requires(IsScalarType<R> && HasComplexFieldGet<T> && !HasSU2Get<T> && !HasSU2DoubletGet<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && IsScalarType<T> && !HasSU2Get<R> && !HasSU2DoubletGet<R>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply<T, R>(t, r);
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && IsScalarType<T> && !HasSU2Get<R> && !HasSU2DoubletGet<R>)
  DEVICE_FORCEINLINE_FUNCTION auto operator/(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply(1_c / t, r);
  }
} // namespace TempLat

#endif
