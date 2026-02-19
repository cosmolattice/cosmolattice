
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/measuringtools/averagerhelper.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

template<typename vType, bool isComplex>
struct AveragerHelperTester {
  static void Test(TDDAssertion &tdd);
};

template <typename vType, bool isComplex> inline void AveragerHelperTester<vType, isComplex>::Test(TDDAssertion &tdd)
{
  /* Default is to fail: to remind yourself to implement something here. */
  // tdd.verify( false ); Tested in averager.
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::AveragerHelperTester<double, false>> test;
}
