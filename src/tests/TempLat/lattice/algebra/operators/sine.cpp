
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/operators/sine.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SineTester {
    static void Test(TDDAssertion &tdd);
  };

  void SineTester::Test(TDDAssertion &tdd)
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
    tdd.verify(AlmostEqual(sin(a).eval(0), std::sin(3.)));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SineTester> test;
}
