#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXWRAPPER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXWRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldoperator.h"
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
    ComplexFieldWrapper() = default;

    DEVICE_FUNCTION
    ComplexFieldWrapper(const R &pR, const T &pT) : mR(pR), mT(pT) {}

    DEVICE_FUNCTION
    ComplexFieldWrapper(const ComplexFieldWrapper &) = default;

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return mR; }
    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return mT; }

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return ComplexFieldGet(t); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::array<decltype(DoEval::eval(mR, idx...)), 2> result;
      result[0] = DoEval::eval(mR, idx...);
      result[1] = DoEval::eval(mT, idx...);
      return result;
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
} // namespace TempLat

#endif
