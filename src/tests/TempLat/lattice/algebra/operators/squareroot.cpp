
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/squareroot.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct SqrtTester {
    static void Test(TDDAssertion &tdd);
  };

  void SqrtTester::Test(TDDAssertion &tdd)
  {

    class myClass
    {
    public:
      DEVICE_FUNCTION
      myClass(int b) : a(b) {}

      DEVICE_FORCEINLINE_FUNCTION
      auto eval(const double &i) const { return i; }

    private:
      [[maybe_unused]] double a;
    };

    myClass a(4);
    // myClass b(4);
    say << sqrt(a).eval(4) << "\n";
    /*
      tdd.verify( AlmostEqual(sqrt(a).get(4), 2)  );



      say << safeSqrt(a).get(4) << "\n";
      tdd.verify( AlmostEqual(safeSqrt(a).get(4),2)  );


      say << safeSqrt(a).get(-1) << "\n";
      tdd.verify( AlmostEqual(safeSqrt(a).get(-1),0)  );
      */
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::SqrtTester> test;
}
