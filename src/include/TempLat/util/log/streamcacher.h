#ifndef TEMPLAT_UTIL_LOG_STREAMCACHER_H
#define TEMPLAT_UTIL_LOG_STREAMCACHER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <sstream>

#include "TempLat/util/log/logmutex.h"
#include "TempLat/util/tdd/tddmacros.h"

namespace TempLat {

    /** \brief A container that sits at the receiving end of a stream,
      *     and spits out everything to the mutexed std::cerr only
      *     when it is destructed -> that the end of the line;.
      * 
      * Unit test: make test-streamcacher
      */

    class StreamCacher {
    public:
    /* Put public methods here. These should change very little over time. */
        StreamCacher() :
        cache(std::make_shared<std::stringstream>())
        {

        }

        ~StreamCacher() {
            if ( cache.use_count() < 2 ) {
                if ( cache->rdbuf()->in_avail() ) {
                    auto guard = LogMutex::guard();
                    std::cout << cache->rdbuf() << "\n";
                }
            }
        }
        
        template <typename T>
        friend StreamCacher operator<< ( StreamCacher stream, const T& other ) {
            *(stream.cache) << other;
            return stream;
        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        std::shared_ptr<std::stringstream> cache;
        


    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/log/streamcacher_test.h"
#endif


#endif
