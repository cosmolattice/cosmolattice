#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNGRID_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNGRID_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/lattice/field/field.h"

inline void TempLat::GetNDim::Test(TempLat::TDDAssertion &tdd)
{
  tdd.verify(GetNDim::get<Field<1, double>>() == 1);
  tdd.verify(GetNDim::get<Field<2, double>>() == 2);
  tdd.verify(GetNDim::get<Field<3, double>>() == 3);
  tdd.verify(GetNDim::get<Field<4, double>>() == 4);
  tdd.verify(GetNDim::get<Field<5, double>>() == 5);
  tdd.verify(GetNDim::get<Field<6, double>>() == 6);

  tdd.verify(GetNDim::get<double>() == 0);
  tdd.verify(GetNDim::get<complex<double>>() == 0);
}

#endif
