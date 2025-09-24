#ifndef COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2DOUBLETMULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2DOUBLETMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldmultiply.h"

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include <Kokkos_Macros.hpp>
#include <type_traits>

namespace TempLat
{
  /** \brief A class which implements the fundamental action of SU2.
   *
   *
   * Unit test: make test-complexfieldsu2multiply
   **/
  template <typename R, typename T> class ComplexFieldSU2DoubletMultiplication : public SU2DoubletBinaryOperator<R, T>
  {
  public:
    using SU2DoubletBinaryOperator<R, T>::mR;
    using SU2DoubletBinaryOperator<R, T>::mT;

    /* Put public methods here. These should change very little over time. */
    ComplexFieldSU2DoubletMultiplication(const R &pR, const T &pT) : SU2DoubletBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<0> t) const { return Real(mR * Complexify(mT.SU2DoubletGet(0_c), mT.SU2DoubletGet(1_c))); }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<1> t) const { return Imag(mR * Complexify(mT.SU2DoubletGet(0_c), mT.SU2DoubletGet(1_c))); }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<2> t) const { return Real(mR * Complexify(mT.SU2DoubletGet(2_c), mT.SU2DoubletGet(3_c))); }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<3> t) const { return Imag(mR * Complexify(mT.SU2DoubletGet(2_c), mT.SU2DoubletGet(3_c))); }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(0_c, idx...);
        mT.SU2DoubletGet(0_c, idx...);
        mR.ComplexFieldGet(1_c, idx...);
        mT.SU2DoubletGet(1_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<0> t, const IDX &...idx) const
    {
      return mR.ComplexFieldGet(0_c, idx...) * mT.SU2DoubletGet(0_c, idx...) -
             mR.ComplexFieldGet(1_c, idx...) * mT.SU2DoubletGet(1_c, idx...);
    }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(0_c, idx...);
        mT.SU2DoubletGet(0_c, idx...);
        mR.ComplexFieldGet(1_c, idx...);
        mT.SU2DoubletGet(1_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<1> t, const IDX &...idx) const
    {
      return mR.ComplexFieldGet(0_c, idx...) * mT.SU2DoubletGet(1_c, idx...) +
             mR.ComplexFieldGet(1_c, idx...) * mT.SU2DoubletGet(0_c, idx...);
    }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(0_c, idx...);
        mT.SU2DoubletGet(2_c, idx...);
        mR.ComplexFieldGet(1_c, idx...);
        mT.SU2DoubletGet(3_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<2> t, const IDX &...idx) const
    {
      return mR.ComplexFieldGet(0_c, idx...) * mT.SU2DoubletGet(2_c, idx...) -
             mR.ComplexFieldGet(1_c, idx...) * mT.SU2DoubletGet(3_c, idx...);
    }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(0_c, idx...);
        mT.SU2DoubletGet(2_c, idx...);
        mR.ComplexFieldGet(1_c, idx...);
        mT.SU2DoubletGet(3_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<3> t, const IDX &...idx) const
    {
      return mR.ComplexFieldGet(0_c, idx...) * mT.SU2DoubletGet(3_c, idx...) +
             mR.ComplexFieldGet(1_c, idx...) * mT.SU2DoubletGet(2_c, idx...);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    virtual std::string operatorString() const override { return "*"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  struct ComplexFieldSU2DoubletMultiplyTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires((HasComplexFieldGet<R> || IsComplexType<R>) && HasSU2DoubletGet<T>)
  auto operator*(const R &r, const T &t)
  {
    return ComplexFieldSU2DoubletMultiplication<R, T>(r, t);
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<T> && HasSU2DoubletGet<R>)
  auto operator*(const R &r, const T &t)
  {
    return ComplexFieldSU2DoubletMultiplication<R, T>(r, t);
  }

  template <typename R, typename T>
    requires((std::is_arithmetic_v<std::decay_t<R>> || HasGetMethod<R>) && HasSU2DoubletGet<T>)
  auto operator*(const R &r, const T &t)
  {
    return ComplexFieldSU2DoubletMultiplication(Complexify(r, ZeroType()), t);
  }

  template <typename R, typename T>
    requires((std::is_arithmetic_v<std::decay_t<R>> || HasGetMethod<R>) && HasSU2DoubletGet<T>)
  auto operator*(const T &t, const R &r)
  {
    return ComplexFieldSU2DoubletMultiplication(Complexify(r, ZeroType()), t);
  }

  template <typename R, typename T>
    requires((std::is_arithmetic_v<std::decay_t<R>> || HasGetMethod<R>) && HasSU2DoubletGet<T>)
  auto operator/(const T &t, const R &r)
  {
    return ComplexFieldSU2DoubletMultiplication(Complexify(1_c / r, ZeroType()), t);
  }
} // namespace TempLat

#endif
