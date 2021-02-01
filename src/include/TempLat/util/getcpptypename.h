#ifndef TEMPLAT_UTIL_GETCPPTYPENAME_H
#define TEMPLAT_UTIL_GETCPPTYPENAME_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/demangle.h"

namespace TempLat {

    /** \brief A class which calls typeid and demangles it if possible.
     *
     * 
     * Unit test: make test-getcpptypename
     **/

    class GetCPPTypeName {
    public:
        
        template <typename T>
        static inline std::string get(const T& instance) {
            return get<T>();
        }

        template <typename T>
        static inline std::string get() {
            return Demangle::demangle(typeid(T).name());
        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/getcpptypename_test.h"
#endif


#endif
