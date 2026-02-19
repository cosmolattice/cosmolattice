
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct GetComponentTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetComponentTester::Test(TDDAssertion &tdd)
  {
    auto v1 = std::make_tuple(87.0, 2);

    tdd.verify(GetComponent::get(v1, Tag<1>()) == 2);
    tdd.verify(GetComponent::get(35, Tag<1>()) == 35);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetComponentTester> test;
}
