#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDCONJUGATE_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDCONJUGATE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/complexalgebra/real.h"
#include "TempLat/lattice/algebra/complexalgebra/imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"

namespace TempLat
{
  /** @brief A class which computes the complex conjugate of a complex field.
   *
   * Unit test: ctest -R test-complexfieldconjugate
   **/
  template <typename R> class ComplexFieldConjugate : public ComplexFieldUnaryOperator<R>
  {
  public:
    // Put public methods here. These should change very little over time.

    using ComplexFieldUnaryOperator<R>::mR;

    DEVICE_FUNCTION
    ComplexFieldConjugate(const R &pR) : ComplexFieldUnaryOperator<R>(pR) {}

    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<0> t) const { return Real(mR); }
    DEVICE_FORCEINLINE_FUNCTION auto ComplexFieldGet(Tag<1> t) const { return -Imag(mR); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      const auto child = DoEval::eval(mR, idx...);
      device::array<std::decay_t<decltype(child[0])>, 2> result;
      result[0] = child[0];
      result[1] = -child[1];
      return result;
    }

    std::string toString() const { return GetString::get(mR) + "^*"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <typename R>
    requires HasComplexFieldGet<R>
  DEVICE_FORCEINLINE_FUNCTION auto conj(const R &r)
  {
    return ComplexFieldConjugate<R>(r);
  }

  template <typename R>
    requires HasComplexFieldGet<R>
  DEVICE_FORCEINLINE_FUNCTION auto dagger(const R &r)
  {
    return ComplexFieldConjugate<R>(r);
  }

  DEVICE_FORCEINLINE_FUNCTION
  OneType dagger(OneType) { return {}; }
  DEVICE_FORCEINLINE_FUNCTION
  OneType conj(OneType) { return {}; }

  DEVICE_FORCEINLINE_FUNCTION
  ZeroType conj(ZeroType t) { return t; };
  DEVICE_FORCEINLINE_FUNCTION
  ZeroType dagger(ZeroType t) { return t; };
} // namespace TempLat

#endif
