#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** \brief A class which implements scalar multiplication over complex numbers.
   *
   * Unit test: make test-scalarcomplexfieldmultiply
   **/

  template <typename R, typename T> class ScalarComplexFieldMultiply : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    KOKKOS_FUNCTION
    ScalarComplexFieldMultiply(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    KOKKOS_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return mR * Real(mT); }

    KOKKOS_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return mR * Imag(mT); }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires VariadicIndex<IDX...>;
        GetValue::get(mR, idx...);
        mT.ComplexFieldGet(0_c, idx...);
      }
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return GetEval::getEval(mR, idx...) * mT.ComplexFieldGet(0_c, idx...);
    }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires VariadicIndex<IDX...>;
        GetValue::get(mR, idx...);
        mT.ComplexFieldGet(1_c, idx...);
      }
    KOKKOS_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return GetEval::getEval(mR, idx...) * mT.ComplexFieldGet(1_c, idx...);
    }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    KOKKOS_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    static std::string operatorString() { return "*"; }
  };

  struct ScalarComplexFieldMultiplyTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires(!HasComplexFieldGet<R> && !IsComplexType<R> && HasComplexFieldGet<T>)
  KOKKOS_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(!HasComplexFieldGet<T> && !IsComplexType<T> && HasComplexFieldGet<R>)
  KOKKOS_FORCEINLINE_FUNCTION auto operator*(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply<T, R>{t, r};
  }

  template <typename R, typename T>
    requires(!HasComplexFieldGet<T> && !IsComplexType<T> && HasComplexFieldGet<R>)
  KOKKOS_FORCEINLINE_FUNCTION auto operator/(const R &r, const T &t)
  {
    return ScalarComplexFieldMultiply<T, R>{1_c / t, r};
  }
} // namespace TempLat

#endif
