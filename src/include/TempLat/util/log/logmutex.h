#ifndef TEMPLAT_UTIL_LOG_LOGMUTEX_H
#define TEMPLAT_UTIL_LOG_LOGMUTEX_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <memory>
#include <mutex>
#include <iostream>

#include "TempLat/util/tdd/tddmacros.h"

namespace TempLat {

    /** \brief A class which holds a mutex, specific for output (log).
      *
      * Unit test: make test-logmutex
      */

    class LogMutex {
    public:
        /* Put public methods here. These should change very little over time. */
        LogMutex() {}
        
        static void lock() {
            std::cerr << "Mutex locking.\n";
            mutex()->lock();
        }

        static void unlock() {
            std::cerr << "Mutex unlocking.\n";
            mutex()->unlock();
        }
        
        static inline std::unique_ptr<std::lock_guard<std::recursive_mutex>> guard() {
            return std::make_unique<std::lock_guard<std::recursive_mutex>>(*mutex());
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

        /** \brief A trick to instantiate the static theList in a header-only file: make it a static variable inside an inline function / method. */
        inline static std::shared_ptr<std::recursive_mutex> mutex() {
            static std::shared_ptr<std::recursive_mutex> mut = std::make_shared<std::recursive_mutex>();
            return mut;
        }



    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/log/logmutex_test.h"
#endif


#endif
