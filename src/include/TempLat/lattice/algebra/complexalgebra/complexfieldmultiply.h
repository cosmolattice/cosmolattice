#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/subtract.h"

namespace TempLat
{
  /** @brief A class which multiplies two complex fields.
   *
   *
   * Unit test: ctest -R test-complexfieldmultiply
   **/
  template <class R, class T> class ComplexFieldMultiplication : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.
    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    DEVICE_FUNCTION
    ComplexFieldMultiplication(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return Real(mR) * Real(mT) - Imag(mR) * Imag(mT); }
    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return Real(mR) * Imag(mT) + Imag(mR) * Real(mT); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      if constexpr (IsComplexType<R>) {
        auto cR = DoEval::eval(mT, idx...);
        device::array<decltype(Real(mR) * cR[0]), 2> result;
        result[0] = Real(mR) * cR[0] - Imag(mR) * cR[1];
        result[1] = Real(mR) * cR[1] + Imag(mR) * cR[0];
        return result;
      } else if constexpr (IsComplexType<T>) {
        auto cL = DoEval::eval(mR, idx...);
        device::array<decltype(cL[0] * Real(mT)), 2> result;
        result[0] = cL[0] * Real(mT) - cL[1] * Imag(mT);
        result[1] = cL[0] * Imag(mT) + cL[1] * Real(mT);
        return result;
      } else {
        auto cL = DoEval::eval(mR, idx...);
        auto cR = DoEval::eval(mT, idx...);
        device::array<decltype(cL[0] * cR[0]), 2> result;
        result[0] = cL[0] * cR[0] - cL[1] * cR[1];
        result[1] = cL[0] * cR[1] + cL[1] * cR[0];
        return result;
      }
    }

    virtual std::string operatorString() const override { return "*"; }
  };

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && HasComplexFieldGet<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ComplexFieldMultiplication<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(IsComplexType<R> && HasComplexFieldGet<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ComplexFieldMultiplication<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && IsComplexType<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ComplexFieldMultiplication<R, T>(r, t);
  }

  template <typename R>
    requires HasComplexFieldGet<R>
  auto norm2(R &&r)
  {
    return pow<2>(r.ComplexFieldGet(0_c)) + pow<2>(r.ComplexFieldGet(1_c));
  }
} // namespace TempLat

#endif
