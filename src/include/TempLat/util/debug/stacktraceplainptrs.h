#ifndef TEMPLAT_UTIL_DEBUG_STACKTRACEPLAINPTRS_H
#define TEMPLAT_UTIL_DEBUG_STACKTRACEPLAINPTRS_H
/* File created by Wessel Valkenburg, 2019 */
/* Released under the MIT license, see LICENSE.md. */


#include "TempLat/util/tdd/tdd.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <signal.h>
#include <cxxabi.h>
#include <thread>
#include <chrono>
#include <vector>
#include <array>
#include <iostream>
    
#ifdef TempLatPOSIXDETECTED
#undef TempLatPOSIXDETECTED
#endif
#ifdef __linux__
#define TempLatPOSIXDETECTED
/* File created by Wessel Valkenburg, 2019 */
/* Released under the MIT license, see LICENSE.md. */

#elif defined(__APPLE__)
#define TempLatPOSIXDETECTED
/* File created by Wessel Valkenburg, 2019 */
/* Released under the MIT license, see LICENSE.md. */

#else
#error "posix detection failed"
#endif



#ifdef TempLatPOSIXDETECTED
#include <unistd.h>
#if defined(__APPLE__)
#include <dlfcn.h>
#include <mach-o/dyld.h>
#endif
#endif
    
    /* undefine this if you cannot compile. */
    //#define HAVE_LIBUNWIND
#ifdef HAVE_LIBUNWIND
#include <libunwind.h>
#define TEMPLATEUNWIND true
#else
#define TEMPLATEUNWIND false
#endif

namespace TempLat {

    /** \brief Templated size: just throw in your array and we fill it to at most its size. Plain backtrace version. */
    template <size_t N>
    inline typename std::enable_if<N != 1u && N != 0u && ! TEMPLATEUNWIND, void>::type StacktracePlainptrs(std::array<void*, N> *result, int *addrlen) {
#ifdef TempLatPOSIXDETECTED
        
        // retrieve current stack addresses
        *addrlen = backtrace(result->data(), N > 1u ? N - 1u : N);
        
        /* N > 1 */
        result->operator[](*addrlen) = NULL;
        
#endif
        return;
        
    }
    
    /** \brief Templated size: just throw in your array and we fill it to at most its size. Libunwind (better!) version. */
    template <size_t N>
    inline typename std::enable_if<N != 1u && N != 0u && TEMPLATEUNWIND, void>::type StacktracePlainptrs(std::array<void*, N> *result, int *addrlen) {
#ifdef HAVE_LIBUNWIND
        
        unw_cursor_t cursor; unw_context_t uc;
        unw_word_t ip;
        //  unw_word_t sp;
        
        unw_getcontext(&uc);
        unw_init_local(&cursor, &uc);
        
        *addrlen = 0;
        while (unw_step(&cursor) > 0 && *addrlen < ((ptrdiff_t) N) - 1) {
            unw_get_reg(&cursor, UNW_REG_IP, &ip);
            //    unw_get_reg(&cursor, UNW_REG_SP, &sp);
            //    printf ("ip = %lx, sp = %lx\n", (long) ip, (long) sp);
            result->operator[]((*addrlen)++) = (void*) ip;
        }
        
        // retrieve current stack addresses
        
        /* N > 1 */
        result->operator[](*addrlen) = NULL;
        
#endif
        return;
        
    }
    
    
    /** \brief A class which
     * Unit test: make test-stacktraceplainptrs
     */
    
}

#ifdef TEMPLATTEST
#include "TempLat/util/debug/stacktraceplainptrs_test.h"
#endif


#endif
