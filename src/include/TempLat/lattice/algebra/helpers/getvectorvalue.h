#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETVECTORVALUE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include <Kokkos_Macros.hpp>

namespace TempLat
{
  /** \brief A getter for the vector-like algebra.
   *
   * Unit test: make test-getvectorvalue
   **/
  class GetVectorValue
  {
  public:
    /* Put public methods here. These should change very little over time. */

    template <typename U, std::integral I, std::integral... JDX>
      requires(HasVectorGetMethod<U> && (sizeof...(JDX) >= 1))
    KOKKOS_FORCEINLINE_FUNCTION static auto vectorGet(U &obj, const I &i, const JDX &...jdx)
    {
      return obj.vectorGet(i, jdx...);
    }

    template <typename U, std::integral I, std::integral... JDX>
      requires(!HasVectorGetMethod<U> && (sizeof...(JDX) >= 1))
    KOKKOS_FORCEINLINE_FUNCTION static auto vectorGet(U &&obj, const I &i, const JDX &...jdx)
    {
      return GetValue::get(obj, i);
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

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getvectorvalue_test.h"
#endif

#endif
