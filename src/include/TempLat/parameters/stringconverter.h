#ifndef TEMPLAT_PARAMETERS_STRINGCONVERTER_H
#define TEMPLAT_PARAMETERS_STRINGCONVERTER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/parameters/multipleparametergetter.h"

namespace TempLat
{
  /** @brief A class which wraps a single function, splitting it in lines and passing each
   *  line to ParameterGetter, returning the result in your provided MultipleParameterGetter<T>.
   *
   * Unit test: ctest -R test-stringconverter
   **/
  template <class T> class StringConverter
  {
  public:
    // Put public methods here. These should change very little over time.
    StringConverter() = default;
    void operator()(const std::string &str, MultipleParameterGetter<T> &arr, const std::string &name)
    {
      arr.clear();
      T tmp;
      std::istringstream iss(str);

      while (iss >> std::boolalpha >> std::skipws >> tmp) {
        arr.push_back(ParameterGetter<T>(tmp, name));
      }
    }
  };
} // namespace TempLat

#endif
