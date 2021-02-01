#ifndef TEMPLAT_UTIL_JSON_FILETOJSON_TEST_H
#define TEMPLAT_UTIL_JSON_FILETOJSON_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/namedtmpfile.h"
#include "TempLat/util/log/log.h"

inline void TempLat::FileToJSON::Test(TempLat::TDDAssertion& tdd) {

    NamedTmpFile ntf;
    
    ntf << "{\"physics\" : {\"q\" : 1e5, \"g2\" : 4e-5, \"startNorm\" : 1e-10, \"fStar\" : 0.1, \"phi\" : { \"randomSeed\" : \"Helllooooo!!!\"}, \"chi\" : { \"randomSeed\" : \"Hello to you too. Entropy perturbations, aye?\"} }, \"integrator\" : { \"tEnd\" : 200, \"dt\" : 0.0002, \"outputFrequency\" : 1, \"conformalPowerAlpha\" : 3, \"scaleFactor_initial\" : 1 }, \"output\" : { \"path\" : \"../demoOutput\" } }";
    
    std::string fName = ntf.getName();
    
    ntf.close();

    FileToJSON ftj(fName);
    
    // say << ftj;
    
    ntf.remove();
    
    tdd.verify( ftj.size() != 0 );

}

#endif
