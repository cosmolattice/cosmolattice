#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2COMMUTATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2COMMUTATOR_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler  Year: 2025

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/variadicindex.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat
{
  /** \brief A class which computes the commutator of two SU(2) matrices.
   *
   * Unit test: make test-su2commutator
   **/
  template <typename R, typename T> class SU2Commutator : public SU2BinaryOperator<R, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using SU2BinaryOperator<R, T>::mR;
    using SU2BinaryOperator<R, T>::mT;

    using SV = typename SU2GetGetReturnType<R>::type;

    SU2Commutator(const R &pR, const T &pT) : SU2BinaryOperator<R, T>(pR, pT) {}

    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<0> t) const { return ZeroType(); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<1> t) const { return 2 * (mR.SU2Get(3_c) * mT.SU2Get(2_c) - mR.SU2Get(2_c) * mT.SU2Get(3_c)); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<2> t) const { return 2 * (mR.SU2Get(1_c) * mT.SU2Get(3_c) - mR.SU2Get(3_c) * mT.SU2Get(1_c)); }
    KOKKOS_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<3> t) const { return 2 * (mR.SU2Get(2_c) * mT.SU2Get(1_c) - mR.SU2Get(1_c) * mT.SU2Get(2_c)); }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    auto SU2Get(const IDX &...idx) const
    {
      return std::move(cache);
    }
    template <int N, typename... IDX> KOKKOS_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t, const IDX &...idx) const
    {
      return cache[N];
    }

    template <typename... IDX>
      requires requires(R mR, T mT, IDX... idx) { VariadicIndex<IDX...>; }
    KOKKOS_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);

      device::array<SV, 4> cL;
      device::array<SV, 4> cR;

      constexpr_for<0, 4, 1>([&](auto j) {
        cL[j] = mR.SU2Get(j, idx...);
        cR[j] = mT.SU2Get(j, idx...);
      });

      cache[0] = 0;
      cache[1] = 2 * (cL[3] * cR[2] - cL[2] * cR[3]);
      cache[2] = 2 * (cL[1] * cR[3] - cL[3] * cR[1]);
      cache[3] = 2 * (cL[2] * cR[1] - cL[1] * cR[2]);
    }

    virtual std::string operatorString() const override { return "commutator"; }

  private:
    mutable device::array<SV, 4> cache;
  };

  struct SU2CommutatorTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires(HasSU2Get<R> && HasSU2Get<T>)
  auto commutator(const R &r, const T &t)
  {
    return SU2Commutator{r, t};
  }

  template <typename T>
    requires HasSU2Get<T>
  auto commutator(OneType r, const T &t)
  {
    return ZeroType();
  }

  template <typename R>
    requires HasSU2Get<R>
  auto commutator(const R &r, OneType t)
  {
    return ZeroType();
  }

  template <typename T>
    requires HasSU2Get<T>
  auto commutator(ZeroType r, const T &t)
  {
    return ZeroType();
  }

  template <typename R>
    requires HasSU2Get<R>
  auto commutator(const R &r, ZeroType t)
  {
    return ZeroType();
  }
} // namespace TempLat

#endif
