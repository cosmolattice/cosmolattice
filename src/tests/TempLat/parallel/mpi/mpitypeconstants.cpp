
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/parallel/mpi/mpitypeconstants.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

struct MPITypeConstantsTester {
  static void Test(TDDAssertion &tdd);
};

void MPITypeConstantsTester::Test(TDDAssertion &tdd) { tdd.verify(true); }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MPITypeConstantsTester> test;
}
