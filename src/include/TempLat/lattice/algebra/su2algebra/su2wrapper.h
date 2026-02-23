#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2operator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/preget.h"
#include "TempLat/lattice/algebra/helpers/postget.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which construct a SU2-like object.
   *
   *
   * Unit test: ctest -R test-su2wrapper
   **/
  template <class A, class B, class C, class D> class SU2Wrapper : public SU2Operator
  {
  public:
    using SV = typename GetGetReturnType<A>::type;

    // Put public methods here. These should change very little over time.
    SU2Wrapper() = default;

    SU2Wrapper(const A &pA, const B &pB, const C &pC, const D &pD) : data(pA, pB, pC, pD) {}

    DEVICE_FUNCTION
    SU2Wrapper(const SU2Wrapper &) = default;

    template <int N> DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t) const { return device::get<N>(data); }
    template <int N> DEVICE_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return SU2Get(t); }

    std::string toString() const
    {
      return "SU2(" + GetString::get(device::get<0>(data)) + "," + GetString::get(device::get<1>(data)) + "," +
             GetString::get(device::get<2>(data)) + "," + GetString::get(device::get<3>(data)) + ")";
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
      device::array<SV, 4> result;
      result[0] = DoEval::eval(device::get<0>(data), idx...);
      result[1] = DoEval::eval(device::get<1>(data), idx...);
      result[2] = DoEval::eval(device::get<2>(data), idx...);
      result[3] = DoEval::eval(device::get<3>(data), idx...);
      return result;
    }

    void preGet()
    {
      PreGet::apply(device::get<0>(data));
      PreGet::apply(device::get<1>(data));
      PreGet::apply(device::get<2>(data));
      PreGet::apply(device::get<3>(data));
    }

    void postGet()
    {
      PostGet::apply(device::get<0>(data));
      PostGet::apply(device::get<1>(data));
      PostGet::apply(device::get<2>(data));
      PostGet::apply(device::get<3>(data));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    device::tuple<std::decay_t<A>, std::decay_t<B>, std::decay_t<C>, std::decay_t<D>> data;
  };

  template <class A, class B, class C, class D> auto SU2Wrap(const A &pA, const B &pB, const C &pC, const D &pD)
  {
    return SU2Wrapper<A, B, C, D>(pA, pB, pC, pD);
  }

  template <typename F> auto SU2Wrap(const F &f) { return SU2Wrap(f(0_c), f(1_c), f(2_c), f(3_c)); }

#define MakeSU2(a, expr) SU2Wrap([&](auto a) { return expr; })
} // namespace TempLat

#endif
