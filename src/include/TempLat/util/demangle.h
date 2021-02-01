#ifndef TEMPLAT_UTIL_DEMANGLE_H
#define TEMPLAT_UTIL_DEMANGLE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


//#include "TempLat/util/tdd/tdd.h"

#include <cstdlib>
#include <memory>
#include <string>

#include "TempLat/util/tdd/tddmacros.h"
#include "TempLat/util/debug/cdemangle.h"

#ifdef __GNUG__
#include <cxxabi.h>
#endif

namespace TempLat {

    /** \brief A class which demangles from internal c++ to source code naming, taken from http://stackoverflow.com/a/4541470/2295722
      *
      * 
      * Unit test: make test-demangle
      */

    class Demangle {
    public:
    /* Put public methods here. These should change very little over time. */

/* from http://stackoverflow.com/a/4541470/2295722 */
#ifdef __GNUG__

        static inline std::string demangle(const char* name) {
            
            int status = -4; // some arbitrary value to eliminate the compiler warning
            
            // enable c++11 by passing the flag -std=c++11 to g++
            std::unique_ptr<char, void(*)(void*)> res {
                abi::__cxa_demangle(name, NULL, NULL, &status),
                std::free
            };
            
            return (status==0) ? res.get() : name ;
        }
        
#else
        
        // does nothing if not g++
        static inline std::string demangle(const char* name) {
            std::stringstream sstream;
            CDemangle(sstream, name);
            return sstream.str();
        }
        
#endif


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/demangle_test.h"
#endif


#endif
