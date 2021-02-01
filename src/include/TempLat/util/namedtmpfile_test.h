#ifndef TEMPLAT_UTIL_NAMEDTMPFILE_TEST_H
#define TEMPLAT_UTIL_NAMEDTMPFILE_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


inline void TempLat::NamedTmpFile::Test(TempLat::TDDAssertion& tdd) {
    
    NamedTmpFile ntf;

    tdd.verify( ntf.outfile.tellp() == 0 );

    ntf << "Hoi!";

    tdd.verify( ntf.outfile.tellp() != 0 );
    tdd.verify( ntf.remove() == 0 );
    tdd.verify( ntf.remove() != 0 );

}

#endif
