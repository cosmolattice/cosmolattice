
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/memory/verbositylevels.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct VerbosityLevelsTester {
    static void Test(TDDAssertion &tdd);
  };

  void VerbosityLevelsTester::Test(TDDAssertion &tdd)
  {
    VerbosityLevels verbosity;

    verbosity.setAllOff();

    tdd.verify(!verbosity.fieldAssignment);

    verbosity.setAllOn();

    tdd.verify(verbosity.fieldAssignment);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::VerbosityLevelsTester> test;
}
