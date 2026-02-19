
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/util/tdd/throws.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ThrowsTester {
    template <typename TestObjectUnknownHere> static void Test(TestObjectUnknownHere &tdd);
  };

  template <typename TestObjectUnknownHere> void ThrowsTester::Test(TestObjectUnknownHere &tdd)
  {
    tdd.verify(Throws<std::runtime_error>([]() { throw std::runtime_error("Test throw: should work."); }));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ThrowsTester> test;
}
