#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2COMMUTATOR_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2COMMUTATOR_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler  Year: 2025

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/util/constexpr_for.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat
{
  /** @brief A class which computes the commutator of two SU(2) matrices.
   *
   * Unit test: ctest -R test-su2commutator
   **/
  template <typename R, typename T> class SU2Commutator : public SU2BinaryOperator<R, T>
  {
  public:
    // Put public methods here. These should change very little over time.

    using SU2BinaryOperator<R, T>::mR;
    using SU2BinaryOperator<R, T>::mT;

    using SV = typename SU2GetGetReturnType<R>::type;

    DEVICE_FUNCTION
    SU2Commutator(const R &pR, const T &pT) : SU2BinaryOperator<R, T>(pR, pT) {}

    DEVICE_FUNCTION
    SU2Commutator(const SU2Commutator &) = default;

    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<0> t) const { return ZeroType(); }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<1> t) const { return 2 * (mR.SU2Get(3_c) * mT.SU2Get(2_c) - mR.SU2Get(2_c) * mT.SU2Get(3_c)); }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<2> t) const { return 2 * (mR.SU2Get(1_c) * mT.SU2Get(3_c) - mR.SU2Get(3_c) * mT.SU2Get(1_c)); }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<3> t) const { return 2 * (mR.SU2Get(2_c) * mT.SU2Get(1_c) - mR.SU2Get(1_c) * mT.SU2Get(2_c)); }

    template <typename... IDX>
      requires requires(std::decay_t<R> r, std::decay_t<T> t, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
        DoEval::eval(t, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      auto cL = DoEval::eval(mR, idx...);
      auto cR = DoEval::eval(mT, idx...);
      device::array<SV, 4> result;
      result[0] = SV(0);
      result[1] = 2 * (cL[3] * cR[2] - cL[2] * cR[3]);
      result[2] = 2 * (cL[1] * cR[3] - cL[3] * cR[1]);
      result[3] = 2 * (cL[2] * cR[1] - cL[1] * cR[2]);
      return result;
    }

    virtual std::string operatorString() const override { return "commutator"; }

  private:
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
