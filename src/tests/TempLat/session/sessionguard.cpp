
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/session/sessionguard.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SessionGuardTester {
    static void Test(TDDAssertion &tdd);
  };

  void SessionGuardTester::Test(TDDAssertion &tdd)
  {

    if (SessionGuard::GetInstanceCounter() < 1) {
      SessionGuard guard(0, NULL, true);
    } else {
      /* there is an instance of MPIGuard in the calling main, which is a good thing. Then we can test if the multiple
       * instantiation protection works. */
      tdd.verify(Throws<SessionGuardInstantiationException>([]() { SessionGuard guard(0, NULL, true); }));
    }
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SessionGuardTester> test;
}
