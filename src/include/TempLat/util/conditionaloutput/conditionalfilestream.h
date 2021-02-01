#ifndef TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALFILESTREAM_H
#define TEMPLAT_UTIL_CONDITIONALOUTPUT_CONDITIONALFILESTREAM_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <fstream>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/util/conditionaloutput/conditionalstream.h"


namespace TempLat {

    MakeException(ConditionalFileStreamError);

    /** \brief A class which adds an owned file stream to the ConditionalStream
     *
     * Unit test: make test-conditionalfilestream
     */
    
    class ConditionalFileStream : public ConditionalStream {
        public:
        /* Put public methods here. These should change very little over time. */
        ConditionalFileStream(const std::string& fname_, bool enabled_, std::ios_base::openmode mode =  std::ios_base::out ) :
        ConditionalStream(stream, enabled_),
        fname(fname_),
        mMode(mode)
        {
            if ( enabled_ ) {
                Enable();
            }
        }
        
        ~ConditionalFileStream() {
            if ( stream.is_open() ) stream.close();
        }
        
        void Enable() {
            if ( ! stream.is_open() ) stream.open(fname, mMode);
            if ( ! stream.is_open() ) throw ConditionalFileStreamError("Could not open \"" + fname + "\"");
        }
        
        friend std::ostream& operator<< (std::ostream& ostream, const ConditionalFileStream& obj) {
            
            ostream << "File(" << obj.fname << ")";
            if ( ! obj.stream.is_open() ) ostream << " (disabled)";
            
            return ostream;
        }

        std::ostream& flush()
        {
            return stream.flush();
        }
        
        
        private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::ofstream stream;
        std::string fname;
        std::ios_base::openmode mMode;
        
        
        
        public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/conditionaloutput/conditionalfilestream_test.h"
#endif


#endif
