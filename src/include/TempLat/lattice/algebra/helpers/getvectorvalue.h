#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"

namespace TempLat
{
  template <typename U, typename... IDX>
  concept TypeHasVectorGet = requires(U obj, IDX... i) { obj.vectorGet(i...); };

  /** \brief A getter for the vector-like algebra.
   *
   * Unit test: make test-getvectorvalue
   **/
  class GetVectorValue
  {
  public:
    /* Put public methods here. These should change very little over time. */

    template <typename U, typename... JDX>
      requires(TypeHasVectorGet<U, JDX...> && (sizeof...(JDX) >= 1))
    KOKKOS_FORCEINLINE_FUNCTION static auto vectorGet(U &obj, const JDX &...jdx)
    {
      return obj.vectorGet(jdx...);
    }

    template <typename U, typename... JDX>
      requires(!TypeHasVectorGet<U, JDX...> && (sizeof...(JDX) >= 1))
    KOKKOS_FORCEINLINE_FUNCTION static auto vectorGet(U &&obj, const JDX &...jdx)
    {
      return GetValue::get(obj, jdx...);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    GetVectorValue() {}

  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#endif
