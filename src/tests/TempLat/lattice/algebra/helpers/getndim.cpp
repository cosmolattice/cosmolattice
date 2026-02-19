
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025
#include "TempLat/lattice/algebra/helpers/getndim.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/field.h"

namespace TempLat
{

  struct GetNDimTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetNDimTester::Test(TDDAssertion &tdd)
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

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetNDimTester> test;
}
