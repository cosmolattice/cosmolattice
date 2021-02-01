#ifndef TEMPLAT_UTIL_DEBUG_CDEMANGLE_H
#define TEMPLAT_UTIL_DEBUG_CDEMANGLE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <cxxabi.h>
#include <iostream>

namespace TempLat {
    
    inline void CDemangle(std::ostream& stream, char *start) {
        
        // allocate string which will be filled with the demangled function name
        size_t funcnamesize = 4096;
        char* funcname = (char*)malloc(funcnamesize);
        
        
        char *begin_name = 0, *begin_offset = 0, *end_offset = 0;
        
#ifdef __APPLE__
        // OSX style stack trace
        for ( char *p = start; *p; ++p )
        {
            if ( p - 1 < start ) continue; /** need to be able to subtract one from p */
            
            if (( *p == '_' ) && ( *(p-1) == ' ' )) {
                begin_name = p-1;
            } else if ( *p == '+' ) {
                begin_offset = p-1;
                *begin_offset = '\0';
                if ( std::string(p).length() > 2) begin_offset += 2;
            }
        }
        
        end_offset = begin_offset + std::string(begin_offset).length();
        
#else // other POSIXes
        // find parentheses and +address offset surrounding the mangled name:
        // ./module(function+0x15c) [0x8048a6d]
        for (char *p = start; *p; ++p)
        {
            if (*p == '(')
            begin_name = p;
            else if (*p == '+')
            begin_offset = p;
            else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }
#endif
        if (begin_name && begin_offset && end_offset
            && begin_name < begin_offset)
        {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';
            
            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():
            
            int status;
            char* ret = abi::__cxa_demangle(begin_name,
                                            funcname, &funcnamesize, &status);
            if (status == 0) {
                funcname = ret; // use possibly realloc()-ed string
                stream << "  " << start << " : " << funcname << " + " << begin_offset << "\n";
            }
            else {
                // demangling failed. Output function name as a C function with
                // no arguments.
                stream << "  " << start << " : " << begin_name << " + " << begin_offset << " [" << (status == -1 ? "alloc fail" : (status == -2 ? "invalid mangled name" : "invalid argument") )<< "]\n";
            }
        }
        else
        {
            // couldn't parse the line? print the whole line.
            stream << start << "  %s (unparsed)\n";
        }
        free(funcname);
        
    }
}

#ifdef TEMPLATTEST
#include "TempLat/util/debug/cdemangle_test.h"
#endif


#endif
