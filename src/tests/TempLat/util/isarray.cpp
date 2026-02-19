/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025

#include "TempLat/util/isarray.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct IsNDArrayTester {
    static void Test(TDDAssertion &tdd);
  };

  void IsNDArrayTester::Test(TDDAssertion &tdd)
  {
    tdd.verify(IsNDArray<std::array<int, 3>, 3> == true);
    tdd.verify(IsNDArray<Kokkos::Array<int, 3>, 3> == true);

    tdd.verify(IsNDArray<std::array<int, 3>, 2> == false);
    tdd.verify(IsNDArray<std::array<int, 3>, 4> == false);
    tdd.verify(IsNDArray<Kokkos::Array<int, 3>, 2> == false);
    tdd.verify(IsNDArray<Kokkos::Array<int, 3>, 4> == false);

    tdd.verify(IsNDArray<int, 3> == false);
    tdd.verify(IsNDArray<double, 5> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::IsNDArrayTester> test;
}
