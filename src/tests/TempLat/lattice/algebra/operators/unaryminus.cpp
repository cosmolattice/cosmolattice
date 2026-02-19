
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/unaryminus.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/constants/halftype.h"

namespace TempLat
{

  struct UnaryMinusTester {
    static void Test(TDDAssertion &tdd);
  };

  class myClass
  {
  public:
    myClass(int b) : a(b) {}

    template <std::integral... IDX> DEVICE_FORCEINLINE_FUNCTION auto eval(const IDX &...i) const { return a; }

  private:
    double a;
  };

  void UnaryMinusTester::Test(TDDAssertion &tdd)
  {
    myClass a(4);
    // myClass b(4);
    tdd.verify(AlmostEqual((-a).eval(0), -4));
    tdd.verify(AlmostEqual((-HalfType()).eval(0, 0, 0), -0.5));
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::UnaryMinusTester> test;
}
