#ifndef TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALSAYSHORT_H
#define TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALSAYSHORT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/log/log.h"

namespace TempLat {
    
    /** \brief A class which only spits out to TTY if enabled == true.
     *
     *
     * Unit test: make test-conditionalsayshort
     */
    
    class ConditionalSayShort {
        public:
        /* Put public methods here. These should change very little over time. */
        ConditionalSayShort(bool enabled_) :
        enabled(enabled_)
        {
            
        }
        
        template <typename T>
        ConditionalSayShort& operator<<(const T& t) {
            if ( enabled ) {
                sayShort << t;
            }
            return *this;
        }
        private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        bool enabled;
        
        
        
        public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/conditionaloutput/conditionalsayshort_test.h"
#endif


#endif
