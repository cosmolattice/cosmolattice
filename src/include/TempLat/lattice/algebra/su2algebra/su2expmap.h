#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2EXPMAP_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2EXPMAP_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/paulivectorsalgebra.h"
#include "TempLat/lattice/algebra/operators/cosine.h"
#include "TempLat/lattice/algebra/operators/sine.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/operators/divide.h"
#include "TempLat/lattice/algebra/su2algebra/su2unaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{

  /** @brief A class which computes the exponential map for su(2).
   *
   *
   * Unit test: ctest -R test-su2expmap
   **/

  template <typename R> class SU2ExpMap : public SU2UnaryOperator<R>
  {
  public:
    using SV = typename SU2GetGetReturnType<R>::type;
    using SU2UnaryOperator<R>::mR;

    SU2ExpMap(const R &pR) : SU2UnaryOperator<R>(pR) {}

    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<0> t) const
    {
      return cos(sqrt(pow<2>(mR.SU2Get(1_c)) + pow<2>(mR.SU2Get(2_c)) + pow<2>(mR.SU2Get(3_c))));
    }

    template <int N>
      requires(N >= 1 && N <= 3)
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<N> t) const
    {
      auto a = sqrt(pow<2>(mR.SU2Get(1_c)) + pow<2>(mR.SU2Get(2_c)) + pow<2>(mR.SU2Get(3_c)));
      return mR.SU2Get(t) / a * sin(a);
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
      device::array<SV, 3> cL = {c[1], c[2], c[3]};
      device::array<SV, 4> result;
      PauliVectorsAlgebra::expmap_inplace(result, cL);
      return result;
    }

    std::string toString() const { return "exp(" + GetString::get(mR) + ")"; }
  };

  template <class R>
    requires HasSU2Get<R>
  auto exp(const R &r)
  {
    return SU2ExpMap<R>(r);
  };

} // namespace TempLat

#endif
