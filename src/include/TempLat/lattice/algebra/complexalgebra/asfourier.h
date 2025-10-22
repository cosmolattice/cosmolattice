#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_ASFOURIER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_ASFOURIER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{
  /** \brief A class which treats a complex field as an object in fourier space.
   *
   * Unit test: make test-asfourier
   **/
  template <typename R> class ComplexFieldAsFourier
  {
  public:
    // Put public methods here. These should change very little over time.
    using mRType = typename ComplexGetGetReturnType<R>::type;

    ComplexFieldAsFourier(const R &pR) : mR(pR) {}

    static constexpr size_t NDim = GetNDim::get<R>();

    template <typename... IDX>
      requires requires(R mR, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        mR.ComplexFieldGet(0_c, idx...);
        mR.ComplexFieldGet(1_c, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto get(const IDX &...idx) const
    {
      return complex<mRType>(mR.ComplexFieldGet(0_c, idx...), mR.ComplexFieldGet(1_c, idx...));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
  };

  struct AsFourierTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename R> ComplexFieldAsFourier<R> asFourier(R &&r)
  {
    return ComplexFieldAsFourier<R>(std::forward<R>(r));
  }
} // namespace TempLat

#endif
