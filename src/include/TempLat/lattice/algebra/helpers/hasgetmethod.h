#ifndef TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_H
#define TEMPLAT_LATTICE_ALGEBRA_HASGETMETHOD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  template <typename U, typename... IDX>
  concept HasGetMethodHelper = requires(std::decay_t<U> u, IDX... idx) { u.get(idx...); };

  /** \brief A concept which determines at compile time whether an object has a method `get`.
   *
   **/
  template <class T>
  concept HasGetMethod = HasGetMethodHelper<T, size_t>                                            //
                         || HasGetMethodHelper<T, size_t, size_t>                                 //
                         || HasGetMethodHelper<T, size_t, size_t, size_t>                         //
                         || HasGetMethodHelper<T, size_t, size_t, size_t, size_t>                 //
                         || HasGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t>         //
                         || HasGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t> //
                         || HasGetMethodHelper<T, size_t, size_t, size_t, size_t, size_t, size_t, size_t>;

  struct HasGetMethodTester {
  public:
#ifdef TEMPLATTEST
    template <typename T> static inline void Test(T &tdd);
#endif
  };

} // namespace TempLat

#endif
