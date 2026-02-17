#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXWRAPPER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXWRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldoperator.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"

#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

namespace TempLat
{
  /** @brief A class which wraps two objects as a complex field.
   *
   *
   * Unit test: ctest -R test-complexwrapper
   **/
  template <class R, class T> class ComplexFieldWrapper : public ComplexFieldOperator
  {
  public:
    // Put public methods here. These should change very little over time.

    DEVICE_FUNCTION
    ComplexFieldWrapper() {}

    DEVICE_FUNCTION
    ComplexFieldWrapper(const R &pR, const T &pT) : mR(pR), mT(pT) {}

    DEVICE_FUNCTION
    ComplexFieldWrapper(const ComplexFieldWrapper &other) : mR(other.mR), mT(other.mT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return mR; }
    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return mT; }

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return ComplexFieldGet(t); }

    template <typename... IDX>
      requires requires(R mR, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        GetValue::get(mR, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t, const IDX &...idx) const
    {
      return GetValue::get(mR, idx...);
    }

    template <typename... IDX>
      requires requires(T mT, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        GetValue::get(mT, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t, const IDX &...idx) const
    {
      return GetValue::get(mT, idx...);
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    void preGet()
    {
      PreGet::apply(mR);
      PreGet::apply(mT);
    }

    void postGet()
    {
      PostGet::apply(mR);
      PostGet::apply(mT);
    }

    std::string toString() const { return "Complex(" + GetString::get(mR) + "," + GetString::get(mT) + ")"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
    T mT;
  };

  template <typename R, typename T>
  DEVICE_FORCEINLINE_FUNCTION ComplexFieldWrapper<R, T> Complexify(const R &r, const T &t)
  {
    return {r, t};
  }

#ifdef TEMPLATTEST
  struct ComplexFieldWrapperTester {
  public:
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
