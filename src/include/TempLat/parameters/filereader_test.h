#ifndef TEMPLAT_PARAMETERS_FILEREADER_TEST_H
#define TEMPLAT_PARAMETERS_FILEREADER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

inline void TempLat::FileReader::Test(TempLat::TDDAssertion& tdd) {

    bool UncommentAndPutTheCorrectFilePathForTheTest =true;
    tdd.verify(UncommentAndPutTheCorrectFilePathForTheTest);
   // FileReader fr;
   // fr("../../src/tests/fcn/parameters/test_filereader.txt");
   // for(size_t i = 0; i < fr.size(); ++i ) say << fr[i];
   // /* Default is to fail: to remind yourself to implement something here. */
   // tdd.verify( fr[0] == "aaa = huhu" );
   // tdd.verify( fr[1] == "bbb=pou" );
   // tdd.verify( fr[2] == "ccc= lop p o" );

}

#endif
