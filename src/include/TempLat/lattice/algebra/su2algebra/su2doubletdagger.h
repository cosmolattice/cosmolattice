#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Franz R. Sattler, Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletunaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletgetgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which compute the hermitean conjugate of Doublets
   *
   *
   * Unit test: ctest -R test-su2doubletdagger
   **/
  template <typename R> class SU2DoubletDagger : public SU2DoubletUnaryOperator<R>
  {
  public:
    using SV = SU2DoubletGetGetReturnType<R>::type;
    using SU2DoubletUnaryOperator<R>::mR;

    /* Put public methods here. These should change very little over time. */
    SU2DoubletDagger(const R &pR) : SU2DoubletUnaryOperator<R>(pR) {}

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<M> t)
    {
      if constexpr (M % 2 == 0)
        return mR.SU2DoubletGet(t);
      else
        return -mR.SU2DoubletGet(t);
    }

    template <int N> DEVICE_FORCEINLINE_FUNCTION const auto &operator()(Tag<N> t) const { return SU2DoubletGet(t); }

    template <int M, typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2DoubletGet(Tag<M>(), idx...); }
    DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<M> t, const IDX &...idx) const
    {
      if constexpr (M % 2 == 0)
        return mR.SU2DoubletGet(t, idx...);
      else
        return -mR.SU2DoubletGet(t, idx...);
    }

    template <int M, typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2DoubletGet(Tag<M>(), idx...); }
    device::array<SV, 4> SU2DoubletGet(const IDX &...idx)
    {
      return {SU2DoubletGet(0_c, idx...), SU2DoubletGet(1_c, idx...), SU2DoubletGet(2_c, idx...),
              SU2DoubletGet(3_c, idx...)};
    }

    template <typename... IDX>
      requires IsVariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx)
    {
      DoEval::eval(mR, idx...);
    }

    virtual std::string toString() const override { return GetString::get(mR) + "^\u2020"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  template <class R>
    requires HasSU2DoubletGet<R>
  auto dagger(const R &r)
  {
    return SU2DoubletDagger<R>(r);
  };

#ifdef TEMPLATTEST
  struct SU2DoubletDaggerTester {
    static inline void Test(TDDAssertion &tdd);
  };
#endif
} // namespace TempLat

#endif
