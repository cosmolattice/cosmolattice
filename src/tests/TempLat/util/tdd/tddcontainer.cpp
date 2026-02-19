
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/tdd/tddcontainer.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  template <class TESTME> struct TDDContainerTester {
    static void Test(TDDAssertion &tdd);
  };

  template <typename T> void TDDContainerTester<T>::Test(TDDAssertion &tdd) { tdd.verify(true); }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TDDContainerTester<TempLat::TDDContainerDummy>> test;
}
