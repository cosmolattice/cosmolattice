#ifndef TEMPLAT_UTIL_LOG_TIMESPENT_H
#define TEMPLAT_UTIL_LOG_TIMESPENT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <chrono>
#include <iostream>
#include "TempLat/util/tdd/tddmacros.h"

namespace TempLat {

    /** \brief A class which measures the wall time since its construction.
      *
      * 
      * Unit test: make test-timespent
      */

    class TimeSpent {
    public:
    /* Put public methods here. These should change very little over time. */
        TimeSpent() : mStart(now()) {

        }
        
        /** \brief Static value which returns the age of the total process */
        static auto getProcessAge() {
            return (now() - getProcessStart()).count();
        }
        
        /** \brief Returns the age of this instance. */
        auto get() const {
            return (now() - mStart).count();
        }
        
        friend
        std::ostream& operator<< ( std::ostream& ostream, const TimeSpent& ts) {
            ostream << ts.get() << "ms";
            return ostream;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        std::chrono::milliseconds mStart;

        static std::chrono::milliseconds now() {
            return std::chrono::duration_cast< std::chrono::milliseconds >(
                                                  std::chrono::system_clock::now().time_since_epoch()
                                                  );
        }
        

        /** \brief A trick to instantiate the static theList in a header-only file: make it a static variable inside an inline function / method. */
        inline static std::chrono::milliseconds getProcessStart() {
            static std::chrono::milliseconds pStart = now();
            return pStart;
        };

    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/log/timespent_test.h"
#endif


#endif
