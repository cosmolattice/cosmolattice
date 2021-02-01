#ifndef TEMPLAT_UTIL_FLATTENTUPLE_TEST_H
#define TEMPLAT_UTIL_FLATTENTUPLE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::FlattenTupleTester::Test(TempLat::TDDAssertion& tdd) {

    auto tup = std::make_tuple(std::make_tuple(1,2),std::make_tuple(3,std::make_tuple(4,"what")));
    auto res = flatten_tuple(tup);

    tdd.verify(std::get<0>(res)==1);
    tdd.verify(std::get<1>(res)==2);
    tdd.verify(std::get<2>(res)==3);
    tdd.verify(std::get<3>(res)==4);
    tdd.verify(std::string(std::get<4>(res))=="what");

}

#endif
