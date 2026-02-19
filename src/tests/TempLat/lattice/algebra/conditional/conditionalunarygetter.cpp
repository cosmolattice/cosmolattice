
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/conditional/conditionalunarygetter.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ConditionalUnaryGetterTester {
    static void Test(TDDAssertion &tdd);
  };

  void ConditionalUnaryGetterTester::Test(TDDAssertion &tdd)
  {
    class MyClass
    {
    public:
      MyClass(int b) : a(b) {}

      auto get(ptrdiff_t i) { return a; }
      auto eval(ptrdiff_t i) { return a; }

    private:
      int a;
    };

    tdd.verify(ConditionalUnaryGetter<MyClass> == true);
    tdd.verify(ConditionalUnaryGetter<double> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ConditionalUnaryGetterTester> test;
}
