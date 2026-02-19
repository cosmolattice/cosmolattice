
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020
#include "TempLat/util/rangeiteration/tagtester.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/staticif.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/rangeiteration/for_in_range.h"

namespace TempLat
{

  struct TagTester {
    static void Test(TDDAssertion &tdd);
  };

  void TagTester::Test(TDDAssertion &tdd)
  {
    Tag<45> t1;
    Tag<5> t2;

    tdd.verify(t1 == 45);

    auto testAdd = [&](Tag<50>) { tdd.verify(true); };

    testAdd(t1 + t2);
    auto testMinus = [&](Tag<40>) { tdd.verify(true); };

    testMinus(t1 - t2);

    auto testTimes = [&](Tag<225>) { tdd.verify(true); };

    testTimes(t1 * t2);

    tdd.verify((Tag<40>() < Tag<50>()) == true);
    tdd.verify((Tag<40>() < Tag<30>()) == false);
    tdd.verify((Tag<40>() < Tag<40>()) == false);

    tdd.verify((Tag<40>() <= Tag<50>()) == true);
    tdd.verify((Tag<40>() <= Tag<30>()) == false);
    tdd.verify((Tag<40>() <= Tag<40>()) == true);

    tdd.verify((Tag<40>() > Tag<50>()) == false);
    tdd.verify((Tag<40>() > Tag<30>()) == true);
    tdd.verify((Tag<40>() > Tag<40>()) == false);

    tdd.verify((Tag<40>() >= Tag<50>()) == false);
    tdd.verify((Tag<40>() >= Tag<30>()) == true);
    tdd.verify((Tag<40>() >= Tag<40>()) == true);

    [[maybe_unused]] auto tt1 = 0_c;
    [[maybe_unused]] auto tt2 = 2_c;
    [[maybe_unused]] auto tt3 = 2_c;

    tdd.verify(IfElse((Tag<30>() < Tag<40>()), 0, 1) == 0);
    tdd.verify(IfElse((tt2 < tt1), 0, 1) == 1);

    tdd.verify(IfElse((IsMore(tt2, tt1)), 0, 1) == 0);
    tdd.verify(IfElse((IsLess(tt2, tt1)), 0, 1) == 1);
    tdd.verify(IfElse((IsLess(tt2, tt3)), 0, 1) == 1);
    tdd.verify(IfElse((IsMore(tt2, tt3)), 0, 1) == 1);
    tdd.verify(IfElse((IsLessOrEqual(tt2, tt3)), 0, 1) == 0);
    tdd.verify(IfElse((IsMoreOrEqual(tt2, tt3)), 0, 1) == 0);

    auto res = Total(j, 1, 3, Total(i, 1, 5, If((IsLess(j, i)), 1)));

    tdd.verify(res == 9);

    // auto res = Total(j, 1, 3, Total(i, 1 ,5, If((j<i), 1))); This doe snot compile, nested lambda's capture not
    // constexpr. With only one Total it does compile.
  }

} // namespace TempLat

namespace
{
  TempLat::TDDContainer<TempLat::TagTester> test;
}
