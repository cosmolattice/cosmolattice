
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Daniel G. Figueroa, Adrien Florio, Francisco Torrenti,  Year: 2019

#include "CosmoInterface/simulationmanager.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

template<size_t NDim>
struct SimulationManagerTester {
  static void Test(TDDAssertion &tdd);
};

template <size_t NDim> inline void SimulationManagerTester<NDim>::Test(TDDAssertion &tdd)
{
  /* Default is to fail: to remind yourself to implement something here. */
  tdd.verify(true);
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SimulationManagerTester<3>> test;
}
