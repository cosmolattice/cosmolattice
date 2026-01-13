#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/lattice/algebra/helpers/hasgetmethod.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include <type_traits>

namespace TempLat
{
  /** \brief A class which implements scalar multiplication over complex numbers.
   *
   * Unit test: make test-scalarcomplexfieldmultiply
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
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        GetValue::get(mR, idx...);
        mT.ComplexFieldGet(0_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return GetEval::getEval(mR, idx...) * mT.ComplexFieldGet(0_c, idx...);
    }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        GetValue::get(mR, idx...);
        mT.ComplexFieldGet(1_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return GetEval::getEval(mR, idx...) * mT.ComplexFieldGet(1_c, idx...);
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

  struct ScalarComplexFieldMultiplyTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename T>
  concept IsScalarType = (std::is_arithmetic_v<T> || HasGetMethod<T>) && !HasComplexFieldGet<T>;

  template <typename R, typename T>
    requires(IsScalarType<R> && HasComplexFieldGet<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && IsScalarType<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply<T, R>(t, r);
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && IsScalarType<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator/(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply(1_c / t, r);
  }
} // namespace TempLat

#endif
