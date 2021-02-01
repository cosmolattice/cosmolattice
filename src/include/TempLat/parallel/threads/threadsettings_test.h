#ifndef TEMPLAT_PARALLEL_THREADS_THREADSETTINGS_TEST_H
#define TEMPLAT_PARALLEL_THREADS_THREADSETTINGS_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::ThreadSettings::Test(TempLat::TDDAssertion& tdd) {

    ptrdiff_t initialThreadCount = ThreadSettings::getMaxThreadCount();
    
    ptrdiff_t initialMPISize = ThreadSettings::getMPILocalSize();
    
    ThreadSettings::setMPILocalSize(initialMPISize * 2);
    
    ptrdiff_t newThreadCount = ThreadSettings::getMaxThreadCount();

    ptrdiff_t manuallyComputedThreadCount = std::max((ptrdiff_t) 1, initialThreadCount / 2);
    

    tdd.verify( manuallyComputedThreadCount == newThreadCount );

    ThreadSettings::setMPILocalSize(initialMPISize);

    say << ThreadSettings::getInstance();
}

#endif
