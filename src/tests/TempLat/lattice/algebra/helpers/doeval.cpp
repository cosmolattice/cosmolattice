
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"
#include "TempLat/lattice/algebra/constants/onetype.h"
#include "TempLat/lattice/algebra/constants/halftype.h"

namespace TempLat
{

  struct DoEvalTester {
    static void Test(TDDAssertion &tdd);
  };

  void DoEvalTester::Test(TDDAssertion &tdd)
  {
    struct myTest {
      DEVICE_FORCEINLINE_FUNCTION
      double eval(ptrdiff_t i)
      {
        ev = true;
        m = 76;
        return m;
      }
      double m;

      bool ev = false;
    } mT1;
    struct myTest2 {
      DEVICE_FORCEINLINE_FUNCTION
      double eval()
      {
        ev = true;
        return 777;
      }

      bool ev = false;
    } mT2;
    struct myTest3 {
      DEVICE_FORCEINLINE_FUNCTION
      double eval(int i, int j, int x)
      {
        ev = true;
        return i + j + x;
      }

      bool ev = false;
    } mT3;

    auto ev1 = DoEval::eval(mT1, 0);
    auto ev2 = DoEval::eval(mT2);
    auto ev3 = DoEval::eval(mT3, 1, 2, 3);
    auto ev4 = DoEval::eval(ZeroType(), 0, 1, 2, 3);
    auto ev5 = DoEval::eval(OneType(), 0, 1, 2, 3);
    auto ev6 = DoEval::eval(5, 0, 1, 2, 3);
    auto ev7 = DoEval::eval(HalfType(), 0, 1, 2, 3);

    tdd.verify(mT1.ev);
    tdd.verify(mT2.ev);
    tdd.verify(mT3.ev);

    tdd.verify(ev1 == 76);
    tdd.verify(ev2 == 777);
    tdd.verify(ev3 == 6);
    tdd.verify(ev4 == 0);
    tdd.verify(ev5 == 1);
    tdd.verify(ev6 == 5);
    tdd.verify(ev7 == 0.5f);
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::DoEvalTester> test;
}
