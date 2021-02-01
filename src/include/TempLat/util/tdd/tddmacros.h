#ifndef TEMPLAT_UTIL_TDD_TDDMACROS_H
#define TEMPLAT_UTIL_TDD_TDDMACROS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019



/* https://stackoverflow.com/a/5897216 */
// #define VA_ARGS(...) , ##__VA_ARGS__
// #define assert(x, ...) ASSERT(#x, __FILE__, __LINE__, x VA_ARGS(__VA_ARGS__))
/* my attempt to get rid of the comma: pass it on to another variadic macro. */


#define MAKEASSERTSTRING_2(x, y) #x ", " #y
#define MAKEASSERTSTRING_1(x) #x

#define MAKEASSERTSTRING_3(x, y, z) MAKEASSERTSTRING_2(x, y) ", " #z
#define MAKEASSERTSTRING_4(_1, _2, _3, _4) MAKEASSERTSTRING_2(_1, _2) ", " MAKEASSERTSTRING_2(_3, _4)
#define MAKEASSERTSTRING_5(_1, _2, _3, _4, _5) MAKEASSERTSTRING_3(_1, _2, _3) ", " MAKEASSERTSTRING_2(_4, _5)
#define MAKEASSERTSTRING_6(_1, _2, _3, _4, _5, _6) MAKEASSERTSTRING_3(_1, _2, _3) ", " MAKEASSERTSTRING_3(_4, _5, _6)
#define MAKEASSERTSTRING_7(_1, _2, _3, _4, _5, _6, _7) MAKEASSERTSTRING_3(_1, _2, _3) ", " MAKEASSERTSTRING_4(_4, _5, _6, _7)

#define GETMAKEASSERTSTRING(_1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME
#define MAKEASSERTSTRING(...) GETMAKEASSERTSTRING(__VA_ARGS__, MAKEASSERTSTRING_7, MAKEASSERTSTRING_6, MAKEASSERTSTRING_5, MAKEASSERTSTRING_4, MAKEASSERTSTRING_3, MAKEASSERTSTRING_2, MAKEASSERTSTRING_1)(__VA_ARGS__)

#ifdef verify
#undef verify
#endif

#define verify(...) ASSERT(MAKEASSERTSTRING(__VA_ARGS__), __FILE__, __LINE__, __VA_ARGS__)

/** \file the TDD setup asserts booleans. But assert is a c/c++ standard
 *  defined keyword. So we use 'verify' rather than 'assert'.
 */

#ifdef TEMPLATTEST
#include "TempLat/util/tdd/tddmacros_test.h"
#endif


#endif
