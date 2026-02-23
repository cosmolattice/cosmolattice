
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2024

#include "TempLat/lattice/algebra/operators/acos.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/almostequal.h"

#include <cmath>

namespace TempLat
{

  struct ACosTester {
    static void Test(TDDAssertion &tdd);
  };

  void ACosTester::Test(TDDAssertion &tdd)
  {
    class myClass
    {
    public:
      DEVICE_FUNCTION myClass(double b) : a(b) {}
      DEVICE_FORCEINLINE_FUNCTION auto eval(const int &i) const { return a; }

    private:
      double a;
    };

    myClass a(0.3);
    tdd.verify(AlmostEqual(acos(a).eval(0), std::acos(0.3)));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::ACosTester> test;
}
