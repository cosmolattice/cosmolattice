#ifndef TEMPLAT_UTIL_NAMEDTMPFILE_H
#define TEMPLAT_UTIL_NAMEDTMPFILE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A class which creates a named temporary file, accepts input, and can close and delete.
     *
     * Unit test: make test-namedtmpfile
     **/

    class NamedTmpFile {
    public:
    /* Put public methods here. These should change very little over time. */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        NamedTmpFile() :
//# removing the deprecation warning here, because the security issue is not an issue for us, this class is for development and testing, not for production.
        name(std::tmpnam(NULL)),
        outfile(name)
        {

        }
        
        NamedTmpFile(std::string presetName) :
        name(presetName),
        outfile(name)
        {

        }
        
        ~NamedTmpFile() {
            close();
        }
        
        const std::string& getName() {
            return name;
        }
        
        template <typename T>
        NamedTmpFile& operator<< (const T& obj) {
            outfile << obj;
            return *this;
        }
        
        int remove() {
            close();
            return std::remove(name.c_str());
        }

        void close() {
            if ( outfile.is_open() ) outfile.close();
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        std::string name;
        std::ofstream outfile;

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/namedtmpfile_test.h"
#endif


#endif
