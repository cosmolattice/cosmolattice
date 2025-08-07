#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/operators/power.h"

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

    ComplexFieldMultiplication(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    auto ComplexFieldGet(Tag<0> t) { return Real(mR) * Real(mT) - Imag(mR) * Imag(mT); }
    auto ComplexFieldGet(Tag<1> t) { return Real(mR) * Imag(mT) + Imag(mR) * Real(mT); }

    auto ComplexFieldGet(Tag<0> t, ptrdiff_t i)
    {
      return ComplexFieldGetter::get(mR, 0_c, i) * ComplexFieldGetter::get(mT, 0_c, i) -
             ComplexFieldGetter::get(mR, 1_c, i) * ComplexFieldGetter::get(mT, 1_c, i);
    }
    auto ComplexFieldGet(Tag<1> t, ptrdiff_t i)
    {
      return ComplexFieldGetter::get(mR, 0_c, i) * ComplexFieldGetter::get(mT, 1_c, i) +
             ComplexFieldGetter::get(mR, 1_c, i) * ComplexFieldGetter::get(mT, 0_c, i);
    }

    void eval(ptrdiff_t i)
    {
      DoEval::eval(mR, i);
      DoEval::eval(mT, i);
    }

    std::string operatorString() const override { return "*"; }
  };

  struct ComplexFieldMultiplyTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && HasComplexFieldGet<T>)
  auto operator*(const R &r, const T &t)
  {
    return ComplexFieldMultiplication<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(IsComplexType<R> && HasComplexFieldGet<T>)
  auto operator*(const R &r, const T &t)
  {
    return ComplexFieldMultiplication<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && IsComplexType<T>)
  auto operator*(const R &r, const T &t)
  {
    return ComplexFieldMultiplication<R, T>(r, t);
  }

  template <typename R>
    requires(HasComplexFieldGet<R> && !std::is_same_v<R, OneType>)
  auto operator*(OneType, const R &r)
  {
    return r;
  }

  template <typename R>
    requires(HasComplexFieldGet<R> && !std::is_same_v<R, OneType>)
  auto operator*(const R &r, OneType)
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
  auto operator*(ZeroType, const R &r)
  {
    return ZeroType();
  }

  template <typename R>
    requires HasComplexFieldGet<R>
  auto norm2(R &&r)
  {
    return pow<2>(r.ComplexFieldGet(0_c)) + pow<2>(r.ComplexFieldGet(1_c));
  }
} // namespace TempLat

#endif
