#ifndef TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALSTREAM_H
#define TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALSTREAM_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <sstream>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief  Because of MPI, we often only want to stream if we are root, etc. To simplify the notation,
     * and simply always be able to write output << ..., we create a wrapper that accepts
     * input with operator<<, and does the 'if'-ing inside that operator.
     *
     * https://stackoverflow.com/a/7694321/2295722
     * Unit test: make test-conditionalstream
     */

    class ConditionalStream {
    public:
    /* Put public methods here. These should change very little over time. */
        ConditionalStream(std::ostream& stream_, bool enabled_) :
        stream(stream_),
        enabled(enabled_)
        { }

        template <typename T>
        ConditionalStream& operator<<(const T& t) {
            if ( enabled ) {
                stream << t;
            }
            return *this;
        }

        void Enable() {
            enabled = true;
        }
        void Disable() {
            enabled = false;
        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        std::ostream& stream;
        bool enabled;



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/conditionaloutput/conditionalstream_test.h"
#endif


#endif
