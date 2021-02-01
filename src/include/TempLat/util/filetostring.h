#ifndef TEMPLAT_UTIL_FILETOSTRING_H
#define TEMPLAT_UTIL_FILETOSTRING_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <fstream>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A class which reads a file into a std::string.
     * Constructor does not work, reading is actually done when
     * the class is cast to a string.
     *
     * 
     * Unit test: make test-filetostring
     **/

    class FileToString {
    public:
    /* Put public methods here. These should change very little over time. */
        FileToString(const std::string fName) :
        mFName(fName),
        didIt(false)
        {

        }
        
        operator std::string() {
            readFile();
            return payload;
        }
        
        friend std::ostream& operator<< (std::ostream& ostream, FileToString& obj) {
            ostream << (std::string) obj ;
            return ostream;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        const std::string mFName;

        std::string payload;

        bool didIt;

        void readFile() {
            if ( didIt ) return;
            didIt = true;
            std::ifstream t(mFName);
            t.seekg(0, std::ios::end);
            size_t size = t.tellg();
            std::string buffer(size, ' ');
            t.seekg(0);
            t.read(&buffer[0], size);
            payload = buffer;
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/filetostring_test.h"
#endif


#endif
