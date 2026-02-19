#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXFIELDGET_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXFIELDGET_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): AdrienFlorio,  Year: 2019

#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/complexalgebra/real.h"
#include "TempLat/lattice/algebra/complexalgebra/imag.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat
{
  /** @brief A class which get real and imaginary part.
   *
   *
   * Unit test: ctest -R test-complexfieldget
   **/
  class ComplexFieldGetter
  {
  public:
    template <typename R>
      requires IsComplexType<R>
    static auto get(R &&r, Tag<0> t)
    {
      return Real(r);
    }

    template <typename R>
      requires IsComplexType<R>
    static auto get(R &&r, Tag<1> t)
    {
      return Imag(r);
    }

    template <typename R, int N>
      requires(!IsComplexType<R> && HasComplexFieldGet<R>)
    static auto get(R &&r, Tag<N> t)
    {
      return r.ComplexFieldGet(t);
    }
  };
} // namespace TempLat

#endif
