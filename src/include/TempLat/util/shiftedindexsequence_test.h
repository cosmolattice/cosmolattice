#ifndef TEMPLAT_UTIL_SHIFTEDINDEXSEQUENCE_TEST_H
#define TEMPLAT_UTIL_SHIFTEDINDEXSEQUENCE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

template<size_t...I>
auto myConvertToTuple(std::index_sequence<I...> seq)
{
  return std::make_tuple(I...);
}


inline void TempLat::ShiftedIndexSequenceTester::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    auto test = myConvertToTuple(shifted_index_sequence<12>(std::make_index_sequence<3>{}));

    tdd.verify( std::get<0>(test) == 12 );
    tdd.verify( std::get<1>(test) == 13 );
    tdd.verify( std::get<2>(test) == 14 );

}

#endif
