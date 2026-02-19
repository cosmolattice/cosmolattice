
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/namedtmpfile.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct NamedTmpFileTester {
    static void Test(TDDAssertion &tdd);
  };

  void NamedTmpFileTester::Test(TDDAssertion &tdd)
  {

    NamedTmpFile ntf;

    tdd.verify(ntf.outfile.tellp() == 0);

    ntf << "Hoi!";

    tdd.verify(ntf.outfile.tellp() != 0);
    tdd.verify(ntf.remove() == 0);
    tdd.verify(ntf.remove() != 0);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::NamedTmpFileTester> test;
}
