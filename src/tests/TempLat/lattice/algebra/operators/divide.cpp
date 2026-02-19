
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/divide.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

namespace TempLat
{

  struct DivideTester {
    static void Test(TDDAssertion &tdd);
  };

  void DivideTester::Test(TDDAssertion &tdd)
  {
    class myClass
    {
    public:
      DEVICE_FUNCTION
      myClass(int b) : a(b) {}

      DEVICE_FORCEINLINE_FUNCTION
      auto eval(ptrdiff_t i) const { return a; }

    private:
      double a;
    };

    myClass a(3);
    myClass b(4);

    //    say << mAdd.get(0, NULL) << " " << GetCPPTypeName::get(decltype(mAdd.get(0, NULL))) << "\n";
    //    say << HasEvalMethod<Operators::Add<GetterGetOffset, GetterGetOffset>> << "\n";
    tdd.verify(HasEvalMethod<Operators::Division<myClass, myClass>> == true);

    tdd.verify(AlmostEqual((a / b).eval(0), 0.75));

    int c = 3, d = 4;
    tdd.verify(HasEvalMethod<decltype(c / d)> == false);

    // pointless, but shuts up the compiler about unused variables:
    c = c + d;
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::DivideTester> test;
}
