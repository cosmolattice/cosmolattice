#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SU2DOUBLETMULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SU2DOUBLETMULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat
{
  /** @brief A class which implemetns SU2 SU2Doublet multiplication.
   *
   * Unit test: ctest -R test-su2su2doubletmultiply
   **/
  template <class R, class T> class SU2SU2DoubletMultiplication : public SU2DoubletBinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using SU2DoubletBinaryOperator<R, T>::mR;
    using SU2DoubletBinaryOperator<R, T>::mT;

    using SV = typename SU2GetGetReturnType<R>::type;

    SU2SU2DoubletMultiplication(const R &pR, const T &pT) : SU2DoubletBinaryOperator<R, T>(pR, pT) {}

    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<0> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(2_c) * mT.SU2DoubletGet(2_c) -
             mR.SU2Get(3_c) * mT.SU2DoubletGet(1_c) - mR.SU2Get(1_c) * mT.SU2DoubletGet(3_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<1> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(3_c) * mT.SU2DoubletGet(0_c) +
             mR.SU2Get(2_c) * mT.SU2DoubletGet(3_c) + mR.SU2Get(1_c) * mT.SU2DoubletGet(2_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<2> t) const
    {
      return -mR.SU2Get(1_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(3_c) * mT.SU2DoubletGet(3_c) -
             mR.SU2Get(2_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(0_c) * mT.SU2DoubletGet(2_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2DoubletGet(Tag<3> t) const
    {
      return -mR.SU2Get(2_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(1_c) * mT.SU2DoubletGet(0_c) +
             mR.SU2Get(0_c) * mT.SU2DoubletGet(3_c) - mR.SU2Get(3_c) * mT.SU2DoubletGet(2_c);
    }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      const auto cL = DoEval::eval(mR, idx...);
      const auto cR = DoEval::eval(mT, idx...);

      device::array<SV, 4> result;
      result[0] = cL[0] * cR[0] + cL[2] * cR[2] - cL[3] * cR[1] - cL[1] * cR[3];
      result[1] = cL[0] * cR[1] + cL[3] * cR[0] + cL[2] * cR[3] + cL[1] * cR[2];
      result[2] = -cL[1] * cR[1] + cL[3] * cR[3] - cL[2] * cR[0] + cL[0] * cR[2];
      result[3] = -cL[2] * cR[1] + cL[1] * cR[0] + cL[0] * cR[3] - cL[3] * cR[2];
      return result;
    }

    virtual std::string operatorString() const override { return "*"; }

  private:
  };

  template <typename R, typename T>
    requires(HasSU2Get<R> && HasSU2DoubletGet<T>)
  auto operator*(const R &r, const T &t)
  {
    return SU2SU2DoubletMultiplication<R, T>(r, t);
  }
} // namespace TempLat

#endif
