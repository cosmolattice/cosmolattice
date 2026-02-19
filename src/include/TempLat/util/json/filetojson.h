#ifndef TEMPLAT_UTIL_JSON_FILETOJSON_H
#define TEMPLAT_UTIL_JSON_FILETOJSON_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/filetostring.h"

#include "TempLat/util/json/simplejson.h"

namespace TempLat
{

  /** @brief A class which reads a file which contains valid JSON,
   * and parses it into a json::JSON object.
   *
   * Unit test: ctest -R test-filetojson
   **/
  class FileToJSON : public json::JSON
  {
  public:
    // Put public methods here. These should change very little over time.
    FileToJSON(const std::string fname) : json::JSON(json::JSON::Load(FileToString(fname))) {}

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };
} // namespace TempLat

#endif
