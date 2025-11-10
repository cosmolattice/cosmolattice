
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/su2algebra/su2field.h"
#include "TempLat/lattice/algebra/su2algebra/su2field_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::SU2FieldBase<1, float>> test1;
  TempLat::TDDContainer<TempLat::SU2FieldBase<2, float>> test2;
  TempLat::TDDContainer<TempLat::SU2FieldBase<3, float>> test3;
  TempLat::TDDContainer<TempLat::SU2FieldBase<4, float>> test4;
  TempLat::TDDContainer<TempLat::SU2FieldBase<1, double>> test5;
  TempLat::TDDContainer<TempLat::SU2FieldBase<2, double>> test6;
  TempLat::TDDContainer<TempLat::SU2FieldBase<3, double>> test7;
  TempLat::TDDContainer<TempLat::SU2FieldBase<4, double>> test8;
} // namespace
