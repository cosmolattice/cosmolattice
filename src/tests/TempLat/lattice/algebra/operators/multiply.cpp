
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/multiply.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct MultiplyTester {
    static void Test(TDDAssertion &tdd);
  };

  void MultiplyTester::Test(TDDAssertion &tdd)
  {
    class myClass
    {
    public:
      DEVICE_FUNCTION
      myClass(int b) : a(b) {}

      DEVICE_FORCEINLINE_FUNCTION
      auto get(ptrdiff_t i) const { return a; }

      DEVICE_FORCEINLINE_FUNCTION
      auto eval(ptrdiff_t i) const { return a; }

    private:
      int a;
    };

    myClass c(3);
    myClass b(2);

    //    say << mAdd.get(0, NULL) << " " << GetCPPTypeName::get(decltype(mAdd.get(0, NULL))) << "\n";
    //    say << HasEvalMethod<Operators::Add<GetterGetOffset, GetterGetOffset>>::value << "\n";
    tdd.verify(HasEvalMethod<Operators::Multiplication<myClass, myClass>> == true);
    tdd.verify((b * c).eval(0) == 6);

    int e = 3, f = 4;
    tdd.verify(HasEvalMethod<decltype(e * f)> == false);

    // pointless, but shuts up the compiler about unused variables:
    e = e + f;
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::MultiplyTester> test;
}
