#ifndef TEMPLAT_UTIL_FUNCTION_H
#define TEMPLAT_UTIL_FUNCTION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A macro to simplify writing lambda function. Don't use it in TempLat, for the interfaces.
     *
     * 
     * Unit test: make test-function
     **/

#define Function(x,expr) [&](auto x){return expr;}

    struct FunctionTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/function_test.h"
#endif


#endif
