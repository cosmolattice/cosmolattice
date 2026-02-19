
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionrebinner.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

template<typename T>
struct RadialProjectionRebinnerTester {
  static void Test(TDDAssertion &tdd);
};

template <typename T> inline void RadialProjectionRebinnerTester<T>::Test(TDDAssertion &tdd)
{

  say << "See fcn/lattice/measuringtoolsIO/radialprojection_test.h\n";
  tdd.verify(true);
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::RadialProjectionRebinnerTester<double>> test;
}
