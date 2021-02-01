#ifndef TEMPLAT_UTIL_NAKEDTYPE_H
#define TEMPLAT_UTIL_NAKEDTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which return the bare type, without references and const
     *
     * 
     * Unit test: make test-nakedtype
     **/

    template<typename R>
    struct NakedType{
        typedef typename std::remove_cv<typename std::remove_reference<R>::type>::type type;
    };

    class NakedTypeTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/nakedtype_test.h"
#endif


#endif
