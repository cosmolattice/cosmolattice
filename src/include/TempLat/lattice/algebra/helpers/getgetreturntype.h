#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETGETRETURNTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat
{
  /** @brief get(IterationCoordinates&) might be a function which returns a real value or a complex
   * value. Sometimes we need to explicitly access what type it is.
   *
   * Unit test: ctest -R test-getgetreturntype
   **/
  template <typename T> struct GetGetReturnType {
    using type = std::decay_t<decltype(TempLat::GetValue::get_example(std::declval<std::decay_t<T>>()))>;
    static constexpr bool isComplex = IsComplexType<type>;
  };

  struct GetGetReturnTypeTester {
#ifdef TEMPLATTEST
    template <typename T> static inline void Test(T &tdd);
#endif
  };
} // namespace TempLat

#endif
