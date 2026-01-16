#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_HASGETVECTORMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio, Franz R. Sattler,  Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include <type_traits>

namespace TempLat
{
  /** \brief A helper concept for HasVectorGetMethod for a given set of indices.
   *
   */
  template <typename U, typename... IDX>
  concept HasVectorGetMethodHelper = requires(std::decay_t<U> u, IDX... idx) { u.vectorGet(idx...); };

  /** \brief A concept which determines at compile time whether an object has a method `vectorGet`.
   *
   **/
  template <class T>
  concept HasVectorGetMethod =
      (HasVectorGetMethodHelper<T, size_t>                                                    //
       || HasVectorGetMethodHelper<T, size_t, size_t>                                         //
       || HasVectorGetMethodHelper<T, size_t, size_t, size_t>                                 //
       || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t>                         //
       || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t>                 //
       || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t>         //
       || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t, size_t> //
       || HasVectorGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t, size_t, size_t>);

  struct HasVectorGetMethodTester {
  public:
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#endif
