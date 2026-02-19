
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/helpers/hasstringmethod.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct HasStringMethodTester {
    static void Test(TDDAssertion &tdd);
  };

  void HasStringMethodTester::Test(TDDAssertion &tdd)
  {
    struct MyTestOne {
      void toString() { std::cerr << "Hell yeah.\n"; }
    };

    struct MyTestTwo {
      void notToString() { std::cerr << "Hell no.\n"; }
    };

    struct MyTestThree {
      void toString(Tag<3> i) { std::cerr << "Hell yes.\n"; }
    };

    tdd.verify(HasStringMethod<MyTestOne> == true);
    tdd.verify(HasStringMethod<MyTestTwo> == false);
    tdd.verify(HasStringMethod<int> == false);
    tdd.verify(HasStringMethodIndexed<3, MyTestOne> == false);
    tdd.verify(HasStringMethodIndexed<3, MyTestThree> == true);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasStringMethodTester> test;
}
