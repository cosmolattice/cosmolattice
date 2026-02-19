
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/debug/stacktrace.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

struct StacktraceTester {
  static void Test(TDDAssertion &tdd);
};

void StacktraceTester::Test(TDDAssertion &tdd)
{

  if (TDDRegister::isSingleUnitTest()) {
    std::cerr << "Stacktrace: " << Stacktrace() << "\n";
  }
}

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::StacktraceTester> test;
}
