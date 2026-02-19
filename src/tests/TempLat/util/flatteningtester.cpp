
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/util/flatteningtester.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct FlatteningTester {
    static void Test(TDDAssertion &tdd);
  };

  struct dummy1 {

    template <int N> auto SU2Get(Tag<N> t) { return N; }

    static constexpr size_t size = 4;
    using Getter = SU2Getter;
  };

  struct dummy2 {

    template <int N> auto getComp(Tag<N> t) { return dummy1(); }
    static constexpr size_t size = 3;
    using Getter = GetComponent;
  };

  struct dummy3 {

    template <int N> auto getComp(Tag<N> t) { return dummy2(); }
    static constexpr size_t size = 5;
    using Getter = GetComponent;
  };

  void FlatteningTester::Test(TDDAssertion &tdd)
  {

    auto dum = dummy3();

    tdd.verify(IsComposite<dummy1>::value == false);
    tdd.verify(IsComposite<dummy2>::value == true);
    tdd.verify(IsComposite<dummy3>::value == true);

    auto testmake = make_tuple_from(dum);

    tdd.verify(tuple_size<decltype(testmake)>::value == 5);
    tdd.verify(tuple_size<decltype(flatten_tuple(testmake))>::value == 60);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::FlatteningTester> test;
}
