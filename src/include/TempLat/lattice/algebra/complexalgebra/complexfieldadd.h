#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDADD_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDADD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/real.h"
#include "TempLat/lattice/algebra/complexalgebra/imag.h"
#include "TempLat/lattice/algebra/operators/add.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"

namespace TempLat
{

  /** @brief A class which implements complex field addition.
   *
   * Unit test: ctest -R test-complexfieldadd
   **/
  template <class R, class T> class ComplexFieldAddition : public ComplexFieldBinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using ComplexFieldBinaryOperator<R, T>::mR;
    using ComplexFieldBinaryOperator<R, T>::mT;

    DEVICE_FUNCTION
    ComplexFieldAddition(const R &pR, const T &pT) : ComplexFieldBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return Real(mR) + Real(mT); }
    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return Imag(mR) + Imag(mT); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto cL = DoEval::eval(mR, idx...);
      auto cR = DoEval::eval(mT, idx...);
      device::array<decltype(cL[0] + cR[0]), 2> result;
      result[0] = cL[0] + cR[0];
      result[1] = cL[1] + cR[1];
      return result;
    }

    virtual std::string operatorString() const override { return "+"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <typename R, typename T>
    requires(HasComplexFieldGet<R> && HasComplexFieldGet<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator+(const R &r, const T &t)
  {
    return ComplexFieldAddition<R, T>{r, t};
  }

  template <typename R, typename T>
    requires(!HasComplexFieldGet<R> && HasComplexFieldGet<T>)
  DEVICE_FORCEINLINE_FUNCTION auto operator+(const R &r, const T &t)
  {
    return ComplexFieldAddition<ComplexFieldWrapper<R, ZeroType>, T>{Complexify(r, ZeroType()), t};
  }

  template <typename R, typename T>
    requires(!HasComplexFieldGet<T> && HasComplexFieldGet<R>)
  DEVICE_FORCEINLINE_FUNCTION auto operator+(const R &r, const T &t)
  {
    return ComplexFieldAddition<R, ComplexFieldWrapper<T, ZeroType>>{r, Complexify(t, ZeroType())};
  }
} // namespace TempLat

#endif
