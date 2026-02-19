
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/helpers/hasexplicitcoordinatedependence.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  namespace TestScratch
  {
    struct DummyWithCoordinateDependence {
      static constexpr bool EXPLICITCOORDINATEDEPENDENCE = true;
    };
    struct DummyWithoutCoordinateDependence {
    };
    struct SecondDummyWithoutCoordinateDependence {
      static constexpr bool EXPLICITCOORDINATEDEPENDENCE = false;
    };
  } // namespace TestScratch

  struct HasExplicitCoordinateDependenceTester {
    static void Test(TDDAssertion &tdd);
  };

  void HasExplicitCoordinateDependenceTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(HasExplicitCoordinateDependence<TestScratch::DummyWithCoordinateDependence>::value);

    tdd.verify(!HasExplicitCoordinateDependence<TestScratch::DummyWithoutCoordinateDependence>::value);

    tdd.verify(!HasExplicitCoordinateDependence<TestScratch::SecondDummyWithoutCoordinateDependence>::value);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasExplicitCoordinateDependenceTester> test;
}
