
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019
#include "TempLat/lattice/algebra/operators/diracdeltafunction.h"
#include "TempLat/util/tdd/tdd.h"

namespace TempLat
{

  struct DiracDeltaFunctionTester {
    static void Test(TDDAssertion &tdd);
  };

  namespace TestScratch
  {
    template <int RETURNVALUE> struct DiracTesterTemplate {
      DEVICE_FORCEINLINE_FUNCTION
      double get(ptrdiff_t i) const { return RETURNVALUE * std::numeric_limits<double>::epsilon(); }
      DEVICE_FORCEINLINE_FUNCTION
      double eval(ptrdiff_t i) const { return RETURNVALUE * std::numeric_limits<double>::epsilon(); }
    };
  } // namespace TestScratch

  void DiracDeltaFunctionTester::Test(TDDAssertion &tdd)
  {
    using namespace TestScratch;
    DiracTesterTemplate<1> positive;
    DiracTesterTemplate<0> zero;
    DiracTesterTemplate<-1> negative;

    /* weird: std::isfinite and std::isinf give the WRONG result in g++ on my machine. So test explicitly. */
    /* not weird. We use -ffast-math. https://stackoverflow.com/a/22931368/2295722 */

    //    tdd.verify( ! std::isfinite( DiracDelta(zero).get(pIterCoords) ) );
    tdd.verify(DiracDelta(zero).eval(0) > std::numeric_limits<double>::max() / 1.1);

    tdd.verify(DiracDelta(positive).eval(0) == 0);

    tdd.verify(DiracDelta(negative).eval(0) == 0);

    //  tdd.verify( std::is_same<PositiveInfinityType, decltype(DiracDelta(ZeroType()))>::value );

    //  tdd.verify( std::is_same<ZeroType, decltype(DiracDelta(OneType()))>::value );
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::DiracDeltaFunctionTester> test;
}
