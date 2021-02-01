#ifndef TEMPLAT_LATTICE_FIELD_HELPERS_HASASTUPLECAT_TEST_H
#define TEMPLAT_LATTICE_FIELD_HELPERS_HASASTUPLECAT_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::HasAsTupleCatTester::Test(TempLat::TDDAssertion& tdd) {


    struct myTuple{
        myTuple() :
        tup(std::make_tuple(1,std::make_tuple(34,0.965)))
        {}

        auto asTupleCat()
        {
            return std::tuple_cat(tup);
        };
        std::tuple<int,std::tuple<int,double> > tup;
    };

    /* Default is to fail: to remind yourself to implement something here. */
    tdd.verify( HasAsTupleCat<myTuple>::value == true );

}

#endif
