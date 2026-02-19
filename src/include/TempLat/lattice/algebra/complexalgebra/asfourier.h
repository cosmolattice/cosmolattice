#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_ASFOURIER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_ASFOURIER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/isvariadicindex.h"
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat
{
  /** @brief A class which treats a complex field as an object in fourier space.
   *
   * Unit test: ctest -R test-asfourier
   **/
  template <typename R> class ComplexFieldAsFourier
  {
  public:
    // Put public methods here. These should change very little over time.
    using mRType = typename ComplexGetGetReturnType<R>::type;

    ComplexFieldAsFourier(const R &pR) : mR(pR) {}

    static constexpr size_t NDim = GetNDim::get<R>();

    template <typename... IDX>
      requires requires(std::decay_t<R> r, IDX... idx) {
        requires IsVariadicIndex<IDX...>;
        DoEval::eval(r, idx...);
      }
    DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...idx) const
    {
      const auto result = DoEval::eval(mR, idx...);
      return complex<mRType>(result[0], result[1]);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    R mR;
  };

  template <typename R> ComplexFieldAsFourier<R> asFourier(R &&r)
  {
    return ComplexFieldAsFourier<R>(std::forward<R>(r));
  }
} // namespace TempLat

#endif
