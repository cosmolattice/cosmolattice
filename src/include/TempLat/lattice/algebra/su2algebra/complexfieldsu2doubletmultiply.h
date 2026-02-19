#ifndef COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2DOUBLETMULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2DOUBLETMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler  Year: 2025

#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldmultiply.h"

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/parallel/device.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"

namespace TempLat
{
  /** @brief A class which implements the fundamental action of SU2.
   *
   *
   * Unit test: ctest -R test-complexfieldsu2multiply
   **/
  template <typename R, typename T> class ComplexFieldSU2DoubletMultiplication : public SU2DoubletBinaryOperator<R, T>
  {
  public:
    using SU2DoubletBinaryOperator<R, T>::mR;
    using SU2DoubletBinaryOperator<R, T>::mT;

    // Put public methods here. These should change very little over time.
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
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto cmplx = DoEval::eval(mR, idx...);   // complex child — now returns array<T, 2>
      auto doublet = DoEval::eval(mT, idx...); // doublet child — returns array<T, 4>
      const auto &re = cmplx[0];
      const auto &im = cmplx[1];
      device::array<decltype(re * doublet[0]), 4> result;
      result[0] = re * doublet[0] - im * doublet[1];
      result[1] = re * doublet[1] + im * doublet[0];
      result[2] = re * doublet[2] - im * doublet[3];
      result[3] = re * doublet[3] + im * doublet[2];
      return result;
    }

    virtual std::string operatorString() const override { return "*"; }
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
    requires((std::is_arithmetic_v<std::decay_t<R>> || HasEvalMethod<R>) && HasSU2DoubletGet<T> &&
             !HasComplexFieldGet<R> && !IsComplexType<std::decay_t<R>> && !HasSU2Get<R> && !HasSU2DoubletGet<R>)
  auto operator*(const R &r, const T &t)
  {
    return ComplexFieldSU2DoubletMultiplication(Complexify(r, ZeroType()), t);
  }

  template <typename R, typename T>
    requires((std::is_arithmetic_v<std::decay_t<R>> || HasEvalMethod<R>) && HasSU2DoubletGet<T> &&
             !HasComplexFieldGet<R> && !IsComplexType<std::decay_t<R>> && !HasSU2Get<R> && !HasSU2DoubletGet<R>)
  auto operator*(const T &t, const R &r)
  {
    return ComplexFieldSU2DoubletMultiplication(Complexify(r, ZeroType()), t);
  }

  template <typename R, typename T>
    requires((std::is_arithmetic_v<std::decay_t<R>> || HasEvalMethod<R>) && HasSU2DoubletGet<T> &&
             !HasComplexFieldGet<R> && !IsComplexType<std::decay_t<R>> && !HasSU2Get<R> && !HasSU2DoubletGet<R>)
  auto operator/(const T &t, const R &r)
  {
    return ComplexFieldSU2DoubletMultiplication(Complexify(1_c / r, ZeroType()), t);
  }
} // namespace TempLat

#endif
