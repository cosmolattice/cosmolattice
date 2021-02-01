#ifndef TEMPLAT_PARAMETERS_MATHPARSER_SPLITTER_TEST_H
#define TEMPLAT_PARAMETERS_MATHPARSER_SPLITTER_TEST_H
#include <string>
#include "TempLat/lattice/algebra/operators/add.h"

 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

inline void TempLat::Splitter::Test(TempLat::TDDAssertion& tdd) {

    /* Default is to fail: to remind yourself to implement something here. */

    std::string t1 = "2*4+(6*(7389*2(2*5+6)))";
    auto res = split(t1,'+');
    tdd.verify(res[0]=="2*4");
    tdd.verify(res[1]=="(6*(7389*2(2*5+6)))");



}

#endif
