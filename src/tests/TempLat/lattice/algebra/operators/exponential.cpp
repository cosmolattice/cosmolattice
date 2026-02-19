
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/exponential.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct ExponentialTester {
    static void Test(TDDAssertion &tdd);
  };

  // #include "TempLat/lattice/algebra/operators/divide.h"

  void ExponentialTester::Test(TDDAssertion &tdd)
  {

    class myClass
    {
    public:
      DEVICE_FUNCTION
      myClass(int b) : a(b) {}

      DEVICE_FORCEINLINE_FUNCTION
      auto eval(const double &i) const { return a; }

    private:
      double a;
    };

    myClass a(3);
    // myClass b(4);
    say << exp(a).eval(0) << "\n";
    tdd.verify(AlmostEqual(exp(a).eval(0), std::exp(3)));
    //    say << exp(a / b).d(a) << "\n";
    //    say << exp(a / b).d(b) << "\n";
    //
    //    /* Default is to fail: to remind yourself to implement something here. */
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ExponentialTester> test;
}
