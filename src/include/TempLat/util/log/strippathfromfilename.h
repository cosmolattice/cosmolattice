#ifndef TEMPLAT_UTIL_LOG_STRIPPATHFROMFILENAME_H
#define TEMPLAT_UTIL_LOG_STRIPPATHFROMFILENAME_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <stdexcept>

#include "TempLat/util/log/trailingzerochar.h"
#include "TempLat/util/tdd/tddmacros.h"

namespace TempLat {

    /** \brief Performed by the compiler, if the const char* is a string at compile time. */
    /* constexpr functions and constexpr constructors are implicitly inline */
    /* inline */
    constexpr const char* StripPathFromFileName( const char* name) noexcept {
        /* a hack https://www.youtube.com/watch?v=fZjYCQ8dzTc&feature=youtu.be&t=3679 putting an unresolved symbol in an exception that is never thrown: this guarantees that this function is truly only evaluated at compiler time, or else an error is raise. */
        /* and https://stackoverflow.com/questions/32401256/how-can-i-do-a-runtime-assert-in-a-constexpr-function */
        constexpr const char* unresolved_char_symbol = NULL;
        const char *result = name;
        for ( const char *i = name, *i_end = TrailingZeroChar(name); i < i_end; ++i) {
            if ( *i == '/' && i + 1 < i_end ) result = i + 1;
        }
        return true ? result : throw std::runtime_error(unresolved_char_symbol);
    }

    struct StripPathFromFileNameTester {
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };

}

#ifdef TEMPLATTEST
#include "TempLat/util/log/strippathfromfilename_test.h"
#endif


#endif
