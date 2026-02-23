#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETWRAPPER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETWRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletoperator.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"

namespace TempLat
{
  /** @brief A class which creates a doublet out of some preexisting objects
   *
   *
   * Unit test: ctest -R test-su2doubletwrapper
   **/
  template <class A, class B, class C, class D>
  class SU2DoubletWrapper
      : public SU2DoubletOperator<std::max(GetNDim::get<A>(),
                                           std::max(GetNDim::get<B>(), std::max(GetNDim::get<C>(), GetNDim::get<D>())))>
  {
  public:
    // Put public methods here. These should change very little over time.
    SU2DoubletWrapper() = default;

    SU2DoubletWrapper(const A &pA, const B &pB, const C &pC, const D &pD) : mData(pA, pB, pC, pD) {}

    DEVICE_FUNCTION
    SU2DoubletWrapper(const SU2DoubletWrapper &) = default;

    template <int N> DEVICE_FORCEINLINE_FUNCTION const auto &SU2DoubletGet(Tag<N> t) const
    {
      return device::get<N>(mData);
    }

    template <typename... IDX>
      requires requires(std::decay_t<A> a, std::decay_t<B> b, std::decay_t<C> c, std::decay_t<D> d, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(a, idx...);
        DoEval::eval(b, idx...);
        DoEval::eval(c, idx...);
        DoEval::eval(d, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      device::array<decltype(DoEval::eval(device::get<0>(mData), idx...)), 4> result;
      result[0] = DoEval::eval(device::get<0>(mData), idx...);
      result[1] = DoEval::eval(device::get<1>(mData), idx...);
      result[2] = DoEval::eval(device::get<2>(mData), idx...);
      result[3] = DoEval::eval(device::get<3>(mData), idx...);
      return result;
    }

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return SU2DoubletGet(t); }

    std::string toString() const
    {
      return "SU2(" + GetString::get(device::get<0>(mData)) + "," + GetString::get(device::get<1>(mData)) + "," +
             GetString::get(device::get<2>(mData)) + "," + GetString::get(device::get<3>(mData)) + ")";
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    device::tuple<A, B, C, D> mData; // to make sure the data is on the device if needed.
  };

  template <class A, class B, class C, class D> auto SU2DoubletWrap(A &&pA, B &&pB, C &&pC, D &&pD)
  {
    return SU2DoubletWrapper<A, B, C, D>(pA, pB, pC, pD);
  }

  template <typename F> auto SU2DoubletWrap(F &&f) { return SU2DoubletWrap(f(0_c), f(1_c), f(2_c), f(3_c)); }

#define MakeSU2Doublet(a, expr) SU2DoubletWrap([&](auto a) { return expr; })
} // namespace TempLat

#endif
