#ifndef TEMPLAT_PARAMETERS_PARAMETERGETTER_TEST_H
#define TEMPLAT_PARAMETERS_PARAMETERGETTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/almostequal.h"

template <typename T>
inline void TempLat::ParameterGetter<T>::Test(TempLat::TDDAssertion& tdd) {
    double d=9.876;

    ParameterGetter<double> pgd(d,"");

    ParameterGetter<int> pgi(d,"xr");


    std::string str = "9.876";

    ParameterGetter<std::string> pgs(str, "str");

    std::string strbis = pgs;

    say << pgs;

    /* Default is to fail: to remind yourself to implement something here. */

    tdd.verify( AlmostEqual(d, pgd()));

    tdd.verify( (int)d == pgi());

    tdd.verify( str == pgs());


}

#endif
