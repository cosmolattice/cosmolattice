
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/hasdoweneedghosts.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct HasDoWeNeedGhostsTester {
    static void Test(TDDAssertion &tdd);
  };

  void HasDoWeNeedGhostsTester::Test(TDDAssertion &tdd)
  {
    struct MyTestOne {
      void doWeNeedGhosts() { std::cerr << "Hell yeah.\n"; }
    };
    struct MyTestTwo {
      void notDoweNeedGHOST() { std::cerr << "Hell no.\n"; }
    };
    struct MyTestThree {
      void doWeNeedGhosts(Tag<3> i) { std::cerr << "Hell yeah.\n"; }
    };

    tdd.verify(HasDoWeNeedGhostsMethod<MyTestOne> == true);
    tdd.verify(HasDoWeNeedGhostsMethod<MyTestTwo> == false);
    tdd.verify(HasDoWeNeedGhostsMethod<int> == false);
    tdd.verify(HasDoWeNeedGhostsMethodIndexed<0, MyTestOne> == false);
    tdd.verify(HasDoWeNeedGhostsMethodIndexed<3, MyTestThree> == true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasDoWeNeedGhostsTester> test;
}
