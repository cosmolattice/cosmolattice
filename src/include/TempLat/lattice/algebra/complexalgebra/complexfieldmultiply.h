#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/lattice/algebra/operators/subtract.h"
#include <Kokkos_Macros.hpp>

namespace TempLat
{
  /** \brief A class which multiplies two complex fields.
   *
   *
   * Unit test: make test-complexfieldmultiply
   **/
  template <class R, class T> class ComplexFieldMultiplication : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */
    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    DEVICE_FUNCTION
    ComplexFieldMultiplication(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return Real(mR) * Real(mT) - Imag(mR) * Imag(mT); }
    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return Real(mR) * Imag(mT) + Imag(mR) * Real(mT); }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        ComplexFieldGetter::get(mR, 0_c, idx...);
        ComplexFieldGetter::get(mT, 0_c, idx...);
        ComplexFieldGetter::get(mR, 1_c, idx...);
        ComplexFieldGetter::get(mT, 1_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return ComplexFieldGetter::get(mR, 0_c, idx...) * ComplexFieldGetter::get(mT, 0_c, idx...) -
             ComplexFieldGetter::get(mR, 1_c, idx...) * ComplexFieldGetter::get(mT, 1_c, idx...);
    }
    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        ComplexFieldGetter::get(mR, 0_c, idx...);
        ComplexFieldGetter::get(mT, 0_c, idx...);
        ComplexFieldGetter::get(mR, 1_c, idx...);
        ComplexFieldGetter::get(mT, 1_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return ComplexFieldGetter::get(mR, 0_c, idx...) * ComplexFieldGetter::get(mT, 1_c, idx...) +
             ComplexFieldGetter::get(mR, 1_c, idx...) * ComplexFieldGetter::get(mT, 0_c, idx...);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    virtual std::string operatorString() const override { return "*"; }
  };

  struct ComplexFieldMultiplyTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
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
    requires(HasComplexFieldGet<R> && !std::is_same_v<R, OneType>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(OneType, const R &r)
  {
    return r;
  }

  template <typename R>
    requires(HasComplexFieldGet<R> && !std::is_same_v<R, ZeroType>)
  auto operator*(const R &r, ZeroType)
  {
    return ZeroType();
  }

  template <typename R>
    requires(HasComplexFieldGet<R> && !std::is_same_v<R, ZeroType>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(ZeroType, const R &r)
  {
    return ZeroType();
  }

  template <typename R>
    requires HasComplexFieldGet<R>
  DEVICE_FORCEINLINE_FUNCTION auto norm2(R &&r)
  {
    return pow<2>(r.ComplexFieldGet(0_c)) + pow<2>(r.ComplexFieldGet(1_c));
  }
} // namespace TempLat

#endif
