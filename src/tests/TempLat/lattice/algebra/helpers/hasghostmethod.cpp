
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/helpers/hasghostmethod.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct HasGhostMethodTester {
    static void Test(TDDAssertion &tdd);
  };

  void HasGhostMethodTester::Test(TDDAssertion &tdd)
  {
    struct MyTestOne {
      ptrdiff_t confirmGhostsUpToDate()
      {
        std::cerr << "Hell yeah.\n";
        return 0;
      }
    };

    struct MyTestTwo {
      ptrdiff_t notConfirmGhostsUpToDate()
      {
        std::cerr << "Hell no.\n";
        return 0;
      }
    };

    struct MyTestThree {
      ptrdiff_t confirmGhostsUpToDate(Tag<3> i)
      {
        std::cerr << "Hell yeah.\n";
        return 0;
      }
    };

    tdd.verify(HasGhostMethod<MyTestOne> == true);
    tdd.verify(HasGhostMethod<MyTestTwo> == false);
    tdd.verify(HasGhostMethod<int> == false);
    tdd.verify(HasGhostMethodIndexed<3, MyTestOne> == false);
    tdd.verify(HasGhostMethodIndexed<3, MyTestThree> == true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasGhostMethodTester> test;
}
