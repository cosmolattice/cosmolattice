
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct HasStaticGetterDummyTester {
    static void Test(TDDAssertion &tdd);
  };

  class MyClass
  {
  public:
    struct Getter {
      template <typename T, int N> static auto get(T &, Tag<N> tag) { return N; }
    };
  };

  void HasStaticGetterDummyTester::Test(TDDAssertion &tdd)
  {
    static_assert(HasStaticGetter<MyClass> == true);

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify(HasStaticGetter<MyClass> == true);
    tdd.verify(HasStaticGetter<double> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasStaticGetterDummyTester> test;
}
