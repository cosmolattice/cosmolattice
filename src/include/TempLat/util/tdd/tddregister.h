#ifndef TEMPLAT_UTIL_TDD_TDDREGISTER_H
#define TEMPLAT_UTIL_TDD_TDDREGISTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019



#include <cstddef>
#include <vector>
#include <tuple>
#include <iostream>

#include "TempLat/util/demangle.h"
#include "TempLat/util/tdd/tddcontainerbase.h"
#include "TempLat/util/tdd/tddmacros.h"

#include "TempLat/util/log/log.h"

namespace TempLat {
    
    /** \brief A class which registers all unit tests in the currently linked binaries,
     *  and runs all the unit tests when called accordingly.
     *
     *
     * Unit test: make test-tddregister
     */
    
    class TDDRegister {
        public:
        /* Put public methods here. These should change very little over time. */
        static void add( TDDContainerBase *ptr, const char *idName ) {
            if ( ptr != NULL && idName != NULL ) {
                theList().emplace_back(ptr, Demangle::demangle(idName), 0);
            }
            
        }
        
        static ptrdiff_t run() {
            /* it's run time! */
//                say << "Will perform " << theList().size() << " test" << (theList().size() > 1u ? "s" : "") << ".\n";
            for ( auto&& it : theList() ) {
                sayShort << "Starting tests for [" << std::get<1>(it) << "]\n";

                std::get<2>(it) = std::get<0>(it)->Test();
                
                sayShort << "Finished tests for [" << std::get<1>(it) << "]\n\n\n";
            }
            return summarize();
        }
        
        static bool isSingleUnitTest() {
            return theList().size() == 1;
        }
        
        private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        /** \brief A trick to instantiate the static theList in a header-only file: make it a static variable inside an inline function / method. */
        inline static std::vector<std::tuple<TDDContainerBase*, std::string, ptrdiff_t>>& theList() {
  
            static std::vector<std::tuple<TDDContainerBase*, std::string, ptrdiff_t>> theList;

            return theList;
        }
        
        static ptrdiff_t summarize() {
            ptrdiff_t result = 0;
            if ( theList().size() > 0u ) {
                
               
                std::string line;
                line.resize(46, '-');
                
                std::string emptyName;
                emptyName.resize(32, ' ');
                emptyName = "\n" + emptyName;
                
                std::cerr << "Summary:\n";
                std::cerr << line << "\n";
                /* dumb, let's just do it twice, first only print succes, then show all failure. */
                for ( ptrdiff_t sucfail = 0; sucfail < 2; ++sucfail) {
                    for ( auto&& it : theList() ) {
                        
                        ptrdiff_t failCount = std::get<2>(it);
                        if ( (failCount && ! sucfail) || ( !failCount && sucfail) ) continue;
                        std::stringstream sstream;
                        
                        auto str =  "[" + std::get<1>(it) + "]";
                        if ( str.size() <= 32u ) str.resize(32, ' ');
                        else str += emptyName;
                        sstream << str << " - " ;
                        if ( failCount > 0 ) {
                            result += failCount;
                            std::string plurals = failCount == 1 ? "" : "s";
                            sstream << failCount << " fail" << plurals << ".\n";
                        } else {
                            sstream << "all passed.\n";
                        }
                        
                        if ( failCount ) std::cerr << KRED << sstream.str() << KRESET;
                        else std::cerr << sstream.str();
                        
                    }
                }
                std::string tot(" Total:");
                tot.resize(32, ' ');
                std::cerr << line << "\n";
                std::cerr << tot << " - ";
                if ( result > 0 ) {
                    std::cerr << result << " fails.\n";
                } else {
                    std::cerr << "all passed.\n";
                }
                
            }
            return result;
        }
        
        
        public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/tdd/tddregister_test.h"
#endif


#endif
