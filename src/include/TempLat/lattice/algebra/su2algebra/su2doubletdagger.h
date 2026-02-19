#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Franz R. Sattler, Year: 2025

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

    template <int M> DEVICE_FORCEINLINE_FUNCTION auto SU2DoubletGet(Tag<M> t) const
    {
      if constexpr (M % 2 == 0)
        return mR.SU2DoubletGet(t);
      else
        return -mR.SU2DoubletGet(t);
    }

    template <int N> DEVICE_FORCEINLINE_FUNCTION const auto &operator()(Tag<N> t) const { return SU2DoubletGet(t); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto child = DoEval::eval(mR, idx...);
      // child[0] = child[0];
      child[1] = -child[1];
      // child[2] = child[2];
      child[3] = -child[3];
      return child;
    }

    virtual std::string toString() const override { return GetString::get(mR) + "^\u2020"; }
  };

  template <class R>
    requires HasSU2DoubletGet<R>
  auto dagger(const R &r)
  {
    return SU2DoubletDagger<R>(r);
  };
} // namespace TempLat

#endif
