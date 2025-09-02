#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DAGGER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DAGGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2unaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"

namespace TempLat
{

  /** \brief A class which implements the hermitian conjugation.
   *
   *
   * Unit test: make test-su2dagger
   **/
  template <typename R> class SU2Dagger : public SU2UnaryOperator<R>
  {
  public:
    using SV = typename SU2GetGetReturnType<R>::type;
    using SU2UnaryOperator<R>::mR;

    /* Put public methods here. These should change very little over time. */
    SU2Dagger(const R &pR) : SU2UnaryOperator<R>(pR) {}

    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<0> t) const { return mR.SU2Get(0_c); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<1> t) const { return -mR.SU2Get(1_c); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<2> t) const { return -mR.SU2Get(2_c); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<3> t) const { return -mR.SU2Get(3_c); }

    template <int N> KOKKOS_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return SU2Get(t); }

    template <typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2Get(0_c, idx...); }
    KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<0> t, const IDX &...idx) const
    {
      return mR.SU2Get(0_c, idx...);
    }
    template <typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2Get(1_c, idx...); }
    KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<1> t, const IDX &...idx) const
    {
      return -mR.SU2Get(1_c, idx...);
    }
    template <typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2Get(2_c, idx...); }
    KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<2> t, const IDX &...idx) const
    {
      return -mR.SU2Get(2_c, idx...);
    }
    template <typename... IDX>
      requires requires(R r, IDX... idx) { r.SU2Get(3_c, idx...); }
    KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<3> t, const IDX &...idx) const
    {
      return -mR.SU2Get(3_c, idx...);
    }

    template <typename... IDX>
      requires requires(R r, IDX... idx) {
        r.SU2Get(0_c, idx...);
        r.SU2Get(1_c, idx...);
        r.SU2Get(2_c, idx...);
        r.SU2Get(3_c, idx...);
      }
    device::array<SV, 4> SU2Get(const IDX &...idx)
    {
      return {SU2Get(0_c, idx...), SU2Get(1_c, idx...), SU2Get(2_c, idx...), SU2Get(3_c, idx...)};
    }

    std::string toString() const { return GetString::get(mR) + "^\u2020"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  struct SU2DaggerTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <class R>
    requires HasSU2Get<R>
  auto dagger(const R &r)
  {
    return SU2Dagger<R>(r);
  };
} // namespace TempLat

#endif
