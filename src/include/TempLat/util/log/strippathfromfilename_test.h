#ifndef TEMPLAT_UTIL_LOG_STRIPPATHFROMFILENAME_TEST_H
#define TEMPLAT_UTIL_LOG_STRIPPATHFROMFILENAME_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <string>

template <typename TestObjectUnknownHere>
inline void TempLat::StripPathFromFileNameTester::Test(TestObjectUnknownHere& tdd) {

    tdd.verify( StripPathFromFileName("/path/to/hypothetical/file") == std::string("file") );

}

#endif
