#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISARITHMETIC_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISARITHMETIC_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler, Year: 2025

#include "TempLat/lattice/algebra/helpers/iscomplextype.h"
#include <type_traits>

namespace TempLat
{
  template <typename T>
  concept IsArithmetic = (std::is_arithmetic_v<std::decay_t<T>> || IsComplexType<std::decay_t<T>>);
}

#endif