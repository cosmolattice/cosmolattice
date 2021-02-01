#ifndef TEMPLAT_PARALLEL_THREADS_THREADSETTINGS_H
#define TEMPLAT_PARALLEL_THREADS_THREADSETTINGS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <thread>
#include <algorithm>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {
    
    /** \brief A class which does bookkeeping, number of threads allowed by hardware, number of threads allowed by user, etc.
     *
     *
     * Unit test: make test-threadsettings
     **/
    
    class ThreadSettings {
    public:
        /* Put public methods here. These should change very little over time. */

        /** \brief Yes, a global variable. Inline, so it remains header only. Constructor is private.
            All you can do with this instance is pass it to a stream:
            `std::cout << ThreadSettings::getInstance() << "\n";`
        */
        static inline ThreadSettings& getInstance() {
            static ThreadSettings store;
            return store;
        }

        static void setMPILocalSize(ptrdiff_t newSize) {
            getInstance().pSetMPILocalSize(newSize);
        }

        static ptrdiff_t getMPILocalSize() {
            return getInstance().pGetMPILocalSize();
        }

        static void setMPIThreadsNotOK() {
            getInstance().pSetMPIThreadsNotOK();
        }

        static ptrdiff_t getMaxThreadCount() {
            return getInstance().pGetMaxThreadCount();
        }

        friend std::ostream& operator<<( std::ostream& stream, const ThreadSettings& fts) {
            stream << "Threading information:\n";
            stream << " - number of cores on machine:                               " << fts.mHardwareNumCores << "\n";
            stream << " - number of mpi processes for this session on this machine: " << fts.mMPILocalSize << "\n";
            //stream << " - resulting maximum number of threads per process:          " << fts.mHardwareAllowedThreadsPerProcess << "\n";
            //stream << " - user specified number of threads per process:             " << fts.mUserAllowedThreadsPerProcess << "\n";
            return stream;
        }
        
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        ptrdiff_t mMPILocalSize;
        ptrdiff_t mHardwareNumCores;
        ptrdiff_t mHardwareAllowedThreadsPerProcess;
        ptrdiff_t mUserAllowedThreadsPerProcess;
        
        ThreadSettings() :
        mMPILocalSize(1),
        mHardwareNumCores(std::thread::hardware_concurrency()),
        mHardwareAllowedThreadsPerProcess(mHardwareNumCores/mMPILocalSize),
        mUserAllowedThreadsPerProcess(mHardwareAllowedThreadsPerProcess)
        {
            
            if ( mHardwareAllowedThreadsPerProcess < 1 ) mHardwareAllowedThreadsPerProcess = 1;
            
        }
        
        void pSetMPILocalSize(ptrdiff_t newSize) {
            mMPILocalSize = newSize > 0 ? newSize : 1 ;
            mHardwareAllowedThreadsPerProcess = mHardwareNumCores / mMPILocalSize;
            if ( mHardwareAllowedThreadsPerProcess < 1 ) mHardwareAllowedThreadsPerProcess = 1;
        }

        ptrdiff_t pGetMPILocalSize() {
            return mMPILocalSize;
        }
        
        void pSetMPIThreadsNotOK() {
            mUserAllowedThreadsPerProcess = 1;
        }

        ptrdiff_t pGetMaxThreadCount() {
            return mHardwareAllowedThreadsPerProcess;
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/threads/threadsettings_test.h"
#endif


#endif
