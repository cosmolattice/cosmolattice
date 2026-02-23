#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2MULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2MULTIPLY_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/constexpr_for.h"

#include "TempLat/lattice/algebra/su2algebra/helpers/paulivectorsalgebra.h"

#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which
   *  implement SU(2) matrix multiplication.
   *
   * Unit test: ctest -R test-su2multiply
   **/
  template <typename R, typename T> class SU2Multiplication : public SU2BinaryOperator<R, T>
  {
  public:
    using SV = typename SU2GetGetReturnType<R>::type;

    using SU2BinaryOperator<R, T>::mR;
    using SU2BinaryOperator<R, T>::mT;

    // Put public methods here. These should change very little over time.
    SU2Multiplication(const R &pR, const T &pT) : SU2BinaryOperator<R, T>(pR, pT) {}

    DEVICE_FUNCTION
    SU2Multiplication(const SU2Multiplication &) = default;

    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<0> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2Get(0_c) - mR.SU2Get(1_c) * mT.SU2Get(1_c) - mR.SU2Get(2_c) * mT.SU2Get(2_c) -
             mR.SU2Get(3_c) * mT.SU2Get(3_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<1> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2Get(1_c) + mR.SU2Get(3_c) * mT.SU2Get(2_c) + mR.SU2Get(1_c) * mT.SU2Get(0_c) -
             mR.SU2Get(2_c) * mT.SU2Get(3_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<2> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2Get(2_c) + mR.SU2Get(2_c) * mT.SU2Get(0_c) + mR.SU2Get(1_c) * mT.SU2Get(3_c) -
             mR.SU2Get(3_c) * mT.SU2Get(1_c);
    }
    DEVICE_FORCEINLINE_FUNCTION
    auto SU2Get(Tag<3> t) const
    {
      return mR.SU2Get(0_c) * mT.SU2Get(3_c) + mR.SU2Get(3_c) * mT.SU2Get(0_c) + mR.SU2Get(2_c) * mT.SU2Get(1_c) -
             mR.SU2Get(1_c) * mT.SU2Get(2_c);
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
      PauliVectorsAlgebra::multiply_inplace(result, cL, cR);
      return result;
    }

    virtual std::string operatorString() const override { return "."; }

  private:
  };

  template <typename R, typename T>
    requires(HasSU2Get<R> && HasSU2Get<T>)
  auto operator*(const R &r, const T &t)
  {
    return SU2Multiplication<R, T>(r, t);
  }
} // namespace TempLat

#endif
