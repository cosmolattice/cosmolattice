
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/log/saycomplete.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

template <typename TestObjectUnknownHere> void SayCompleteTest::Test(TestObjectUnknownHere &tdd)
{

  say << "Stream log - first light.\n";
  sayShort << "Shorter stream log - first light.\n";
  tdd.verify(true);
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SayCompleteTest> test;
}
