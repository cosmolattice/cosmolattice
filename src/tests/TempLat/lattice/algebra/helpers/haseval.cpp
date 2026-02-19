
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/haseval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/halftype.h"

namespace TempLat
{

  struct HasEvalTester {
    static void Test(TDDAssertion &tdd);
  };

  void HasEvalTester::Test(TDDAssertion &tdd)
  {
    struct myTest {
      DEVICE_FORCEINLINE_FUNCTION
      double eval(ptrdiff_t i)
      {
        m = 76;
        return m;
      }
      double m;
    };
    struct myTest2 {
      DEVICE_FORCEINLINE_FUNCTION
      double eval() { return 777; }
    };
    struct myTest3 {
      DEVICE_FORCEINLINE_FUNCTION
      double eval(int i, int j, int x) { return i + j + x; }
    };

    tdd.verify(HasEval<myTest, int> == true);
    tdd.verify(HasEval<myTest2, int> == false);
    tdd.verify(HasEval<myTest3, ptrdiff_t, ptrdiff_t, ptrdiff_t> == true);
    tdd.verify(HasEval<double> == false);

    tdd.verify(TypeHasStaticValue<myTest> == false);
    tdd.verify(TypeHasStaticValue<ZeroType> == true);
    tdd.verify(TypeHasStaticValue<OneType> == true);
    tdd.verify(TypeHasStaticValue<HalfType> == true);

    tdd.verify(TypeEvalsItself<double> == true);
    tdd.verify(TypeEvalsItself<complex<double>> == true);
    tdd.verify(TypeEvalsItself<ZeroType> == false);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::HasEvalTester> test;
}
