
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/parallel/mpi/comm/mpicommreference.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct MPICommReferenceTester {
    static void Test(TDDAssertion &tdd);
  };

  void MPICommReferenceTester::Test(TDDAssertion &tdd) { tdd.verify(true); }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MPICommReferenceTester> test;
}
