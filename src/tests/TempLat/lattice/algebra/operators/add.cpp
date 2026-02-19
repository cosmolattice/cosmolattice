
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/add.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/operators.h"
#include "TempLat/util/getcpptypename.h"

namespace TempLat
{

  struct AddTester {
    static void Test(TDDAssertion &tdd);
  };

  void AddTester::Test(TDDAssertion &tdd)
  {
    //    GetterGetOffset a, b;
    //    Operators::Add<GetterGetOffset, GetterGetOffset> mAdd(a, b);

    class myClass
    {
    public:
      DEVICE_FUNCTION
      myClass(int b) : a(b) {}

      DEVICE_FORCEINLINE_FUNCTION
      auto get(ptrdiff_t i) const { return a; }

    private:
      int a;
    };

    class myClass2
    {
    public:
      DEVICE_FUNCTION
      myClass2(int b) : a(b) {}

      DEVICE_FORCEINLINE_FUNCTION
      double get(ptrdiff_t i) const { return a; }

      DEVICE_FORCEINLINE_FUNCTION
      auto getDx() const { return 2.89; }

      DEVICE_FORCEINLINE_FUNCTION
      auto getKIR() const { return 9.89; }

    private:
      int a;
    };
    /*
      myClass a(3);
      myClass b(4);
      myClass2 e(4);

      //    say << mAdd.get(0, NULL) << " " << GetCPPTypeName::get(decltype(mAdd.get(0, NULL))) << "\n";
      //    say << HasGetMethod<Operators::Add<GetterGetOffset, GetterGetOffset>>::value << "\n";
      tdd.verify(HasGetMethod<Operators::Addition<myClass, myClass>>::value == true);
      tdd.verify((a+b).get(0) == 7);
      tdd.verify((a+b).getDx() == 1);
      tdd.verify((a+b).getKIR() == 1);
      tdd.verify((a+e).getDx() == 2.89);
      tdd.verify((a+e).getKIR() == 9.89);

      int c = 3, d = 4;
      tdd.verify(HasGetMethod<decltype(c + d)>::value == false);
      // pointless, but shuts up the compiler about unused variables:
      c = c + d;*/
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::AddTester> test;
}
