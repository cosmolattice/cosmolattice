
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/lattice/algebra/operators/cosh.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct CoshTester {
    static void Test(TDDAssertion &tdd);
  };

  void CoshTester::Test(TDDAssertion &tdd)
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
    say << cosh(a).eval(0) << "\n";
    tdd.verify(AlmostEqual(cosh(a).eval(0), std::cosh(3.)));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::CoshTester> test;
}
