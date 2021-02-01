#ifndef TEMPLAT_UTIL_FILETOSTRING_TEST_H
#define TEMPLAT_UTIL_FILETOSTRING_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/namedtmpfile.h"

inline void TempLat::FileToString::Test(TempLat::TDDAssertion& tdd) {
    
    NamedTmpFile ntf;
    
    std::string payload = "Hello world!";
    
    ntf << payload;
    
    ntf.close();
    
    FileToString fts(ntf.getName());
    
    std::string ftsRead = fts;

    tdd.verify( ftsRead == payload );

    tdd.verify( ntf.remove() == 0 );
}

#endif
