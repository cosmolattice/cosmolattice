
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/parallel/mpi/cartesian/mpicartesianneighbours.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct MPICartesianNeighboursTester {
    static void Test(TDDAssertion &tdd);
  };

  void MPICartesianNeighboursTester::Test(TDDAssertion &tdd)
  {

    /* sorry, wouldn't know what cross checks to do for this thing.
       Problems will show up in the integral test in ghostupdater. */
    //    tdd.verify( true );
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MPICartesianNeighboursTester> test;
}
