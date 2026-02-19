
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/hasvectorgetmethod.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct dummy {
    char a;
  };

  struct MyTestOne {
    dummy vectorGet(Tag<1> t)
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };

  struct MyTestTwo {
    void noGet() { std::cerr << "Hell no.\n"; }
  };

  struct MyTestThree {
    template <int N> dummy vectorGet(Tag<N> t)
    {
      std::cerr << "Hell yeah.\n";
      return dummy();
    }
  };

  struct HasVectorGetMethodTester {
    static void Test(TDDAssertion &tdd);
  };

  void HasVectorGetMethodTester::Test(TDDAssertion &tdd)
  {
    tdd.verify(HasVectorGetMethod<MyTestOne> == true);
    tdd.verify(HasVectorGetMethod<MyTestTwo> == false);
    tdd.verify(HasVectorGetMethod<MyTestThree> == true);
    tdd.verify(HasVectorGetMethod<int> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasVectorGetMethodTester> test;
}
