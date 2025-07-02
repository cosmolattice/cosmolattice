#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  /** \brief A concept which determines at compile time whether an object has a method `get`.
   *
   */
  template <typename U, typename... IDX>
  concept HasVectorGetMethodHelper = requires(U u, IDX... idx) { u.vectorGet(idx...); };

  /** \brief A concept which determines at compile time whether an object has a method `get`.
   *
   **/
  template <class T>
  concept HasVectorGetMethod =
      HasVectorGetMethodHelper<T, size_t>                                                    //
      || HasVectorGetMethodHelper<T, size_t, size_t>                                         //
      || HasVectorGetMethodHelper<T, size_t, size_t, size_t>                                 //
      || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t>                         //
      || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t>                 //
      || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t>         //
      || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t, size_t> //
      || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>;

  struct HasVectorGetMethodTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod_test.h"
#endif

#endif
