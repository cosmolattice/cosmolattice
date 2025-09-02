#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SUM_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** \brief A class which implements the sum of su2 matrices.
   *
   *
   * Unit test: make test-su2sum
   **/
  template <typename R, typename T> class SU2Addition : public SU2BinaryOperator<R, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using SU2BinaryOperator<R, T>::mR;
    using SU2BinaryOperator<R, T>::mT;

    using SV = typename SU2GetGetReturnType<R>::type;

    SU2Addition(const R &pR, const T &pT) : SU2BinaryOperator<R, T>(pR, pT) {}

    template <int N> auto SU2Get(Tag<N> t) const { return mT.SU2Get(t) + mR.SU2Get(t); }
    template <int N> KOKKOS_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const { return SU2Get(t); }

    template <typename... IDX> struct RightIndices {
      static constexpr bool value = requires(R r, T t, IDX... idx) {
        r.SU2Get(0_c, idx...);
        r.SU2Get(1_c, idx...);
        r.SU2Get(2_c, idx...);
        r.SU2Get(3_c, idx...);
        t.SU2Get(0_c, idx...);
        t.SU2Get(1_c, idx...);
        t.SU2Get(2_c, idx...);
        t.SU2Get(3_c, idx...);
      };
    };

    template <int N, typename... IDX>
      requires RightIndices<IDX...>::value
    KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t, const IDX &...idx) const
    {
      return mT.SU2Get(t, idx...) + mR.SU2Get(t, idx...);
    }
    template <int N, typename... IDX>
      requires RightIndices<IDX...>::value
    device::array<SV, 4> SU2Get(const IDX &...i) const
    {
      return {SU2Get(0_c, i...), SU2Get(1_c, i...), SU2Get(2_c, i...), SU2Get(3_c, i...)};
    }

    virtual std::string operatorString() const override { return "+"; }
  };

  struct SU2SumTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires(HasSU2Get<R> && HasSU2Get<T>)
  auto operator+(const R &r, const T &t)
  {
    return SU2Addition{r, t};
  }
} // namespace TempLat

#endif
