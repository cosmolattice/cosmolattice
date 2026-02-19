#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DAGGER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DAGGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2019

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2unaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat
{
  /** @brief A class which implements the hermitian conjugation.
   *
   *
   * Unit test: ctest -R test-su2dagger
   **/
  template <typename R> class SU2Dagger : public SU2UnaryOperator<R>
  {
  public:
    using SV = typename SU2GetGetReturnType<R>::type;
    using SU2UnaryOperator<R>::mR;

    // Put public methods here. These should change very little over time.
    SU2Dagger(const R &pR) : SU2UnaryOperator<R>(pR) {}

    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<0> t) const { return mR.SU2Get(Tag<0>()); }

    template <int N>
      requires(N >= 1 && N <= 3)
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t) const
    {
      if constexpr (N == 0) {
        return mR.SU2Get(Tag<N>());
      } else {
        return -mR.SU2Get(Tag<N>());
      }
    }

    template <int N>
      requires(N >= 0 && N <= 3)
    DEVICE_FORCEINLINE_FUNCTION auto operator()(Tag<N> t) const
    {
      return SU2Get(t);
    }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto c = DoEval::eval(mR, idx...);
      c[1] = -c[1];
      c[2] = -c[2];
      c[3] = -c[3];
      return c;
    }

    std::string toString() const { return GetString::get(mR) + "^\u2020"; }
  };

  template <class R>
    requires HasSU2Get<R>
  auto dagger(const R &r)
  {
    return SU2Dagger<R>(r);
  };

  template <class R>
    requires HasSU2Get<R>
  auto dag(const R &r)
  {
    return dagger(r);
  };
} // namespace TempLat

#endif
