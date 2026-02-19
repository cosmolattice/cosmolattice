
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct MPICartesianGroupTester {
    static void Test(TDDAssertion &tdd);
  };

  void MPICartesianGroupTester::Test(TDDAssertion &tdd)
  {

    auto world = MPICommReference();

    MPICartesianGroup mcg(world, 3, {{1, 1, 1}});

    MPICartesianGroup mcg2(world, 1, {{2, 2, 2}});

    tdd.verify(true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MPICartesianGroupTester> test;
}
