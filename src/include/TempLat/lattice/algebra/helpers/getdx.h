#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETDX_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETDX_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasdx.h"
#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat
{
  /** \brief A getter for dx.
   *
   * Unit test: make test-getdx
   **/
  class GetDx
  {
  public:
    /* Put public methods here. These should change very little over time. */

    template <typename U>
      requires HasDx<U>
    KOKKOS_FORCEINLINE_FUNCTION static auto getDx(U &&obj)
    {
      return obj.getDx();
    }

    template <typename U>
      requires(!HasDx<U>)
    KOKKOS_FORCEINLINE_FUNCTION static constexpr double getDx(U &obj)
    {
      return 1;
    }

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
