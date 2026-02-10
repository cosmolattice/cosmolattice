
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/operators/shift_test.h"

namespace
{
  TempLat::TDDContainer<TempLat::ExpressionShifterTester<1>> test1;
  TempLat::TDDContainer<TempLat::ExpressionShifterTester<2>> test2;
  TempLat::TDDContainer<TempLat::ExpressionShifterTester<3>> test3;
  TempLat::TDDContainer<TempLat::ExpressionShifterTester<4>> test4;
} // namespace
