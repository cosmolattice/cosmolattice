#ifndef TEMPLAT_UTIL_FOREACH_TEST_H
#define TEMPLAT_UTIL_FOREACH_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019




void applyStuff(std::string& i)
{
  i="What do you get if you multiply six by nine?";
}

void applyStuff(int& i)
{
  i = 42;
}


inline void TempLat::ForEachTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    auto mTuple = std::make_tuple(1,2,std::string("str"));

    for_each(mTuple, [](auto& x){applyStuff(x);});

    auto mTuple2 = std::make_tuple(1,2,std::string(" 42"));


    tdd.verify( std::get<2>(mTuple) ==  "What do you get if you multiply six by nine?" );
    tdd.verify( std::get<1>(mTuple) == 42 );
  //  tdd.verify( std::get<int>(mTuple) == 42 );
    tdd.verify( std::get<0>(mTuple) == 42);

    binary_for_each(mTuple , mTuple2 , [](auto& x, auto& y){x += y;});

    tdd.verify( std::get<2>(mTuple) == "What do you get if you multiply six by nine? 42" );


}

#endif
