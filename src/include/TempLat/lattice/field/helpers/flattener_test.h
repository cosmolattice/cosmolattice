#ifndef TEMPLAT_LATTICE_FIELD_HELPERS_FLATTENER_TEST_H
#define TEMPLAT_LATTICE_FIELD_HELPERS_FLATTENER_TEST_H
#include "hasastuplecat.h"

 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::Flattener::Test(TempLat::TDDAssertion& tdd) {

    struct myTuple{
        myTuple() :
                tup(std::make_tuple(std::make_tuple(1),std::make_tuple(34,0.965)))
        {}

        auto asTupleCat()
        {
            return std::tuple_cat(std::get<0>(tup),std::get<1>(tup));
        };
        std::tuple<std::tuple<int>,std::tuple<int,double> > tup;
    };

    myTuple te;

    say << HasAsTupleCat<decltype(te)>::value;

    auto test = Flattener::asTupleCat(te);

    say << std::get<0>(test);
    say << std::get<1>(test);
    say << std::get<2>(test);



}

#endif
