
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/operators/cosine.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct CosineTester {
    static void Test(TDDAssertion &tdd);
  };

  void CosineTester::Test(TDDAssertion &tdd)
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
    say << cos(a).eval(0) << "\n";
    tdd.verify(AlmostEqual(cos(a).eval(0), std::cos(3.)));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::CosineTester> test;
}
