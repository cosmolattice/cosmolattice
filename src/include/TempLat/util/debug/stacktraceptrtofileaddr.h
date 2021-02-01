#ifndef TEMPLAT_UTIL_DEBUG_STACKTRACEPTRTOFILEADDR_H
#define TEMPLAT_UTIL_DEBUG_STACKTRACEPTRTOFILEADDR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/debug/stacktraceplainptrs.h"

namespace TempLat {
    
    /** \brief Posix's backtrace gives current memory addresses, which are good for runtime business, but not useful for
     finding source code locations. Need to convert that to file addresses, using this offset. */
    inline ptrdiff_t SetStacktracePtrToFileAddress() {
#if defined(__APPLE__)
        /* get the address of this function. */
        std::array<void*, 128> addrlist;
        int len;
        StacktracePlainptrs(&addrlist, &len);
        if ( len < 1 ) return 0;
        
        void* ourPos = addrlist.begin();
        
        Dl_info info;
        info.dli_fname = NULL;
        info.dli_fbase = NULL;
        info.dli_sname = NULL;
        info.dli_saddr = NULL;
        /* get the info! */
        int success = 0;
        auto it = addrlist.begin();
        while (success == 0 && it < addrlist.end() ) {
            success = dladdr(ourPos, &info);
            it++;
        }
        //  if ( success == 0 ) std::cerr << "Address not found by dladdr.\n";
        //  else {
        //    std::cerr << "Info from dladdr:\nfname: " << (info.dli_fname != NULL ? info.dli_fname : "NULL") << "\nfbase: " << info.dli_fbase << "\nsname: " << (info.dli_sname != NULL ? info.dli_sname : "NULL") << "\nsaddr: " << info.dli_saddr << "\n";
        //  }
        
        int count = _dyld_image_count();
        //  std::cerr << "_dyld_image_count: " << count << "\n";
        //  for ( int i = 0; i < count; ++i) {
        //    std::cerr << "_dyld_get_image_name(i): " << _dyld_get_image_name(i) << "\n";
        //    std::cerr << "_dyld_get_image_vmaddr_slide(i): " << _dyld_get_image_vmaddr_slide(i) << "\n";
        //    std::cerr << "_dyld_get_image_header(i): " << _dyld_get_image_header(i) << "\n";
        //
        //  }
        if ( count ) return (ptrdiff_t) _dyld_get_image_vmaddr_slide(0);
        return (ptrdiff_t) info.dli_fbase;
#endif
        return 0;
    }
    
    inline ptrdiff_t StacktracePtrToFileAddress() {
        static ptrdiff_t addr = SetStacktracePtrToFileAddress();
        return addr;
    }
}

#ifdef TEMPLATTEST
#include "TempLat/util/debug/stacktraceptrtofileaddr_test.h"
#endif


#endif
