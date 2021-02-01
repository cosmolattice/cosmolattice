#ifndef TEMPLAT_UTIL_CONDITIONALOUTPUT_OUTPUTSTREAM_H
#define TEMPLAT_UTIL_CONDITIONALOUTPUT_OUTPUTSTREAM_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/conditionaloutput/conditionalfilestream.h"

namespace TempLat {



    /** \brief A class which handles output, with convenient savetxt function.
     *
     *
     * Unit test: make test-outputstream
     **/

    template <typename R>
    class OutputStream  : public ConditionalFileStream {
    public:
        /* Put public methods here. These should change very little over time. */
        OutputStream(const std::string& fname_, bool enabled_, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
        ConditionalFileStream(fname_, enabled_ , mode) {

        }

    void savetxt() {
      (*this)<<"\n";
    }

    template <typename T, typename... Args>
    void savetxt(T t, Args... args) {
      (*this)<< static_cast<R>(t) <<" ";
      savetxt(args...);
    }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/conditionaloutput/outputstream_test.h"
#endif


#endif
