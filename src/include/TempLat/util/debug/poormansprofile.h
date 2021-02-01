#ifndef TEMPLAT_UTIL_DEBUG_POORMANSPROFILE_H
#define TEMPLAT_UTIL_DEBUG_POORMANSPROFILE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <vector>
#include <array>
#include <tuple>
#include <map>
#include <string>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/debug/stacktrace.h"

namespace TempLat {
    
    /** \brief A class for the poor man's profiling: prints out the map upon destruction.
     *
     * Unit test: make test-poormansprofile
     */
    
    class PoorMansProfile {
        public:
        /* Put public methods here. These should change very little over time. */
        PoorMansProfile() {
            
        }
        
        ~PoorMansProfile() {
            
            /* sort by count, and then print out */
            std::vector<std::pair<std::string, size_t>> sortedList;
            sortedList.reserve(theMap.size());
            for ( auto&& it : theMap ) {
                sortedList.push_back( it );
            }
            std::sort(sortedList.begin(), sortedList.end(), [](std::pair<std::string, size_t>& a, std::pair<std::string, size_t>& b)
                      {
                          return a.second < b.second;
                      });
            
            for ( auto&& it : sortedList) {
                std::cerr << it.second << " - " << it.first << "\n";
            }
        }
        
        
        void Add (const std::string& value) {
            if ( theMap.count(value) == 0 ) theMap[value] = 0;
            ++theMap[value];
        }

        /** \brief And here our poor mans profiler: call this function to manually track where the most calls to a function come from. */
         static inline void TrackMe() {
            static PoorMansProfile myMap;
            myMap.Add(Stacktrace(2, 1));
        }

        private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::map<std::string, size_t> theMap;
        
        
        
        public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/debug/poormansprofile_test.h"
#endif


#endif
