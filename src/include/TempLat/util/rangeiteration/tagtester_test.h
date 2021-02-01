#ifndef TEMPLAT_UTIL_RANGEITERATION_TAGTESTER_TEST_H
#define TEMPLAT_UTIL_RANGEITERATION_TAGTESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/staticif.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/rangeiteration/for_in_range.h"


inline void TempLat::TagTester::Test(TempLat::TDDAssertion& tdd) {



    Tag<45> t1;
    Tag<5> t2;

    tdd.verify( t1 == 45 );

    auto testAdd = [&](Tag<50> )
    {
        tdd.verify(true);
    };

    testAdd(t1+t2);
    auto testMinus = [&](Tag<40> )
    {
        tdd.verify(true);
    };

    testMinus(t1-t2);

    auto testTimes = [&](Tag<225> )
    {
        tdd.verify(true);
    };

    testTimes(t1*t2);

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

    auto tt1 = 0_c;
    auto tt2 = 2_c;
    auto tt3 = 2_c;


    tdd.verify(static_if<(Tag<30>() < Tag<40>()) >( 0 ,1) == 0);
    tdd.verify(static_if<(tt2 < tt1) >( 0 ,1) == 1);


    tdd.verify(static_if<(IsLess(tt2, tt1)) >( 0 ,1) == 1);
    tdd.verify(static_if<(IsMore(tt2, tt1)) >( 0 ,1) == 0);
    tdd.verify(static_if<(IsLess(tt2, tt3)) >( 0 ,1) == 1);
    tdd.verify(static_if<(IsMore(tt2, tt3)) >( 0 ,1) == 1);
    tdd.verify(static_if<(IsLessOrEqual(tt2, tt3)) >( 0 ,1) == 0);
    tdd.verify(static_if<(IsMoreOrEqual(tt2, tt3)) >( 0 ,1) == 0);

    auto res = Total(j, 1, 3, Total(i, 1 ,5, If((IsLess(j,i)), 1)));

    tdd.verify(res == 9);

    //auto res = Total(j, 1, 3, Total(i, 1 ,5, If((j<i), 1))); This doe snot compile, nested lambda's capture not constexpr. With only one Total it does compile.

}

#endif
