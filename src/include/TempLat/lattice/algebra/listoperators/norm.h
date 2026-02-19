#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_NORM_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_NORM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/algebra/operators/squareroot.h"
#include "TempLat/lattice/algebra/operators/power.h"
#include "TempLat/lattice/algebra/listoperators/total.h"

namespace TempLat
{
  /** @brief A class which computes the norm of a list.
   *
   * Unit test: ctest -R test-norm
   **/
  template <typename R>
    requires(IsTempLatGettable<0, R> || IsSTDGettable<0, R>)
  auto norm2(const R &r)
  {
    return total(pow<2>(r));
  }
} // namespace TempLat

#endif
