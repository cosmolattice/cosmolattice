#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTSQUAREROOT_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTSQUAREROOT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "listpower.h"

namespace TempLat
{

  template <typename R>
    requires(IsSTDGettable<0, R> || IsTempLatGettable<0, R>)
  ListPower<R, HalfType> sqrt(const R &r)
  {
    return ListPower<R, HalfType>(r, HalfType());
  }
} // namespace TempLat

#endif
