#ifndef TEMPLAT_UTIL_STATICWARNING_H
#define TEMPLAT_UTIL_STATICWARNING_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A compile-time warning based on template programming values. Taken from https://www.reddit.com/r/cpp/comments/66o1ju/does_a_static_warn_exist/.
     *
     * Unit test: make test-staticwarning
     **/

    template<bool condition>
    struct warn_if{};
    
    template<> struct [[deprecated]] warn_if<false>{constexpr warn_if() = default;};
    
#define static_warn(x, ...) ((void) warn_if<x>())

//int main() {
//    static_warn(false, "This is bad");
//}
    class StaticWarning {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/staticwarning_test.h"
#endif


#endif
