#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SUBTRACT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SUBTRACT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/memory/memorytoolbox.h"

namespace TempLat
{
  /** \brief A class which implements the SU2 Subtraction.
   *
   * Unit test: make test-su2sum
   **/
  template <typename R, typename T> class SU2Subtraction : public SU2BinaryOperator<R, T>
  {
  public:
    /* Put public methods here. These should change very little over time. */

    using SU2BinaryOperator<R, T>::mR;
    using SU2BinaryOperator<R, T>::mT;

    using SV = typename SU2GetGetReturnType<R>::type;

    SU2Subtraction(const R &pR, const T &pT) : SU2BinaryOperator<R, T>(pR, pT) {}

    template <int N> auto SU2Get(Tag<N> t) { return mR.SU2Get(t) - mT.SU2Get(t); }

    template <int N> auto operator()(Tag<N> t) { return SU2Get(t); }

    template <typename... IDX> struct RightIndices {
      static constexpr bool value = requires(R r, T t, IDX... idx) {
        r.SU2Get(0_c, idx...);
        r.SU2Get(1_c, idx...);
        r.SU2Get(2_c, idx...);
        r.SU2Get(3_c, idx...);
      };
    };

    template <typename... IDX>
      requires RightIndices<IDX...>::value
    DEVICE_FORCEINLINE_FUNCTION device::array<SV, 4> SU2Get(const IDX &...idx) const
    {
      return {SU2Get(0_c, idx...), SU2Get(1_c, idx...), SU2Get(2_c, idx...), SU2Get(3_c, idx...)};
    }

    template <int M, typename... IDX>
      requires RightIndices<IDX...>::value
    DEVICE_FORCEINLINE_FUNCTION auto SU2Get(Tag<M> t, const IDX &...idx) const
    {
      return mR.SU2Get(t, idx...) - mT.SU2Get(t, idx...);
    }

    template <typename... IDX>
      requires VariadicIndex<IDX...>
    DEVICE_FORCEINLINE_FUNCTION void eval(const IDX &...idx) const
    {
      DoEval::eval(mR, idx...);
      DoEval::eval(mT, idx...);
    }

    virtual std::string operatorString() const override { return "-"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

  struct SU2SubtractTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R, typename T>
    requires(HasSU2Get<R> && HasSU2Get<T>)
  auto operator-(const R &r, const T &t)
  {
    return SU2Subtraction{r, t};
  }
} // namespace TempLat

#endif
