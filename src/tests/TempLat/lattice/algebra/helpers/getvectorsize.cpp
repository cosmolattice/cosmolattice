
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/getvectorsize.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct GetVectorSizeTester {
    static void Test(TDDAssertion &tdd);
  };

  void GetVectorSizeTester::Test(TDDAssertion &tdd)
  {
    /* Default is to fail: to remind yourself to implement something here. */
    struct dummy {
      char a;
    };

    struct MyTestOne {
      double vectorGet(Tag<1> t)
      {
        std::cerr << "Hell yeah.\n";
        return 420;
      }
      ptrdiff_t getVectorSize() { return 42; }
    };

    MyTestOne t1;

    tdd.verify(GetVectorSize::getVectorSize(t1) == 42);
    tdd.verify(GetVectorSize::getVectorSize(42) == 1);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::GetVectorSizeTester> test;
}
