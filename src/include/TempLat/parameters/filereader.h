#ifndef TEMPLAT_PARAMETERS_FILEREADER_H
#define TEMPLAT_PARAMETERS_FILEREADER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <fstream>
#include "TempLat/util/exception.h"
#include "TempLat/util/stringtrimmer.h"

namespace TempLat
{
  MakeException(FileReaderProblemInputFile);

  /** @brief A class which reads input files.
   *
   *
   * Unit test: ctest -R test-filereader
   **/
  class FileReader
  {
  public:
    // Put public methods here. These should change very little over time.
    FileReader() = default;
    const std::string &operator[](int i) const { return vec[i]; }
    size_t size() const { return vec.size(); }
    void operator()(const std::string &str, char comment = '#')
    {
      std::ifstream t;
      t.open(str);
      std::string tmp;
      if (t.good()) {
        while (getline(t, tmp)) {
          StringTrimmer::trim(tmp);
          if (tmp != "" && tmp[0] != comment) vec.push_back(tmp);
        }
      } else
        throw(FileReaderProblemInputFile("There was a problem opening the input file at " + str + ". Abort."));
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */

    std::vector<std::string> vec;
  };
} // namespace TempLat

#endif
