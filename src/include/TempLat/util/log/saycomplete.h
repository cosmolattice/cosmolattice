#ifndef TEMPLAT_UTIL_LOG_SAYCOMPLETE_H
#define TEMPLAT_UTIL_LOG_SAYCOMPLETE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <string>

#include "TempLat/util/log/streamcacher.h"
#include "TempLat/util/log/colors.h"
#include "TempLat/util/log/timespent.h"
#include "TempLat/util/log/strippathfromfilename.h"
#include "TempLat/util/tdd/tddmacros.h"

namespace TempLat {


    /** \brief Instantiate a container that eats up all the stream that you put in it,
     only locking the mutex and throwing up into std::cerr upon destruction.
     Provide the filename and line number of the place of invocation.
     Use the macro 'say' defined below.
      *
      *
      * Unit test: make test-saycomplete
     */
    inline StreamCacher SayComplete(const char* fname, int line ) {
        StreamCacher result;
        
        long long int ms = TimeSpent::getProcessAge();
        
        auto ss = ms / 1000;
        ms = ms % 1000;
        
        char msBuf[6];
        snprintf(msBuf, 6, "%03lli", ms );
        
        result << KBLU "(" << ss << "." << msBuf << "s) ";
        if ( fname != NULL ) result << fname << ":" << line << " -->" ;
        result << "\n" KRESET;
        return result;
    }
    /** \brief A macro to put time and file info as prompt before the actual output. */
#define say SayComplete(StripPathFromFileName(__FILE__), __LINE__)
#define sayShort SayComplete(NULL, 0)
#define sayMPI SayComplete((std::string("MPI Rank ") + std::to_string(TempLat::MPICommReference::worldRank()) + " - " + std::string(StripPathFromFileName(__FILE__))).c_str(), __LINE__)



    /** \brief A class which just does the test.
      * Unit test: make test-saycomplete
      */

    class SayCompleteTest {
    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/log/saycomplete_test.h"
#endif


#endif
