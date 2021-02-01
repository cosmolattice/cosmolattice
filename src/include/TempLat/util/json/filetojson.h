#ifndef TEMPLAT_UTIL_JSON_FILETOJSON_H
#define TEMPLAT_UTIL_JSON_FILETOJSON_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"

#include "TempLat/util/filetostring.h"

#include "TempLat/util/json/simplejson.h"

namespace TempLat {

    /** \brief A class which reads a file which contains valid JSON,
     * and parses it into a json::JSON object.
     *
     * Unit test: make test-filetojson
     **/

    class FileToJSON : public json::JSON {
    public:
    /* Put public methods here. These should change very little over time. */
        FileToJSON(const std::string fname) :
        json::JSON(json::JSON::Load(FileToString(fname)))
        {

        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/json/filetojson_test.h"
#endif


#endif
