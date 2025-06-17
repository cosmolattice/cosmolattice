#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/complex.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{
  template <typename T>
  concept IsComplexType = std::is_same_v<std::decay_t<T>, complex<typename std::decay_t<T>::value_type>>;

  /** \brief a mini tester class... */
  struct IsComplexTypeTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/iscomplextype_test.h"
#endif

#endif
