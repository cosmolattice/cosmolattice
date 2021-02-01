#ifndef TEMPLAT_UTIL_RANGEITERATION_TAGLISTTESTER_TEST_H
#define TEMPLAT_UTIL_RANGEITERATION_TAGLISTTESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

//Okay to define something globally here as not included anywhere
#include "TempLat/util/rangeiteration/for_in_range.h"

template<int N>
int get(TempLat::Tag<N> t)
{
  return N;
}

template<int N>
int get()
{
  return N;
}

inline void TempLat::TagListTester::Test(TempLat::TDDAssertion& tdd) {


      int c =1;
      for_in_range<1,25>(
        [&](auto i){
          tdd.verify(get(i) == c++);
        }
      );
      c=1;
      for_in_range<1,25>(
        [&](auto i){
          tdd.verify(get<3*i>() == 3*c);
          c++;
        }
      );


}

#endif
