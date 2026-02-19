#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_U1WRAPPER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_U1WRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"

namespace TempLat
{
  /** @brief Exponential map for U(1).
   *
   * Unit test: ctest -R test-u1wrapper
   **/
  template <typename R> class U1Exponential : public ComplexFieldUnaryOperator<R>
  {
  public:
    using SV = typename GetGetReturnType<R>::type;
    // Put public methods here. These should change very little over time.
    using ComplexFieldUnaryOperator<R>::mR;

    U1Exponential(const R &pR) : ComplexFieldUnaryOperator<R>(pR) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<0> t) const { return cos(mR); }
    DEVICE_FORCEINLINE_FUNCTION
    auto ComplexFieldGet(Tag<1> t) const { return sin(mR); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      const auto val = DoEval::eval(mR, idx...);
      return device::array<decltype(val), 2>{cos(val), sin(val)};
    }

    std::string toString() const { return "U1(" + GetString::get(mR) + ")"; }
  };

  template <typename R> auto complexPhase(R &&r) { return U1Exponential<R>(std::forward<R>(r)); }
} // namespace TempLat

#endif
