#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSUBTRACT_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSUBTRACT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/util/tdd/tdd.h"
#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/complexalgebra/real.h"
#include "TempLat/lattice/algebra/complexalgebra/imag.h"

namespace TempLat
{
  /** @brief A class which implements complex subtraction.
   *
   *
   * Unit test: ctest -R test-complexfieldadd
   **/
  template <class R, class T> class ComplexFieldSubtraction : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    // Put public methods here. These should change very little over time.
    ComplexFieldSubtraction(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return Real(mR) - Real(mT); }

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return Imag(mR) - Imag(mT); }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(0_c, idx...);
        mT.ComplexFieldGet(0_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return mR.ComplexFieldGet(0_c, idx...) - mT.ComplexFieldGet(0_c, idx...);
    }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(1_c, idx...);
        mT.ComplexFieldGet(1_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return mR.ComplexFieldGet(1_c, idx...) - mT.ComplexFieldGet(1_c, idx...);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx)
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    virtual std::string operatorString() const override { return "-"; }
  };

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && HasComplexFieldGet<T>)
  auto operator-(const R &r, const T &t)
  {
    return ComplexFieldSubtraction<R, T>{r, t};
  }

  template <typename R, typename T>
    requires(!HasComplexFieldGet<R> && HasComplexFieldGet<T>)
  auto operator-(const R &r, const T &t)
  {
    return ComplexFieldSubtraction{Complexify(r, ZeroType()), t};
  }

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && !HasComplexFieldGet<T>)
  auto operator-(const R &r, const T &t)
  {
    return ComplexFieldSubtraction{r, Complexify(t, ZeroType())};
  }

#ifdef TEMPLATTEST
  struct ComplexFieldSubtractTester {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
