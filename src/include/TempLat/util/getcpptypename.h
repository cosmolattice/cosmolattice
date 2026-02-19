#ifndef TEMPLAT_UTIL_GETCPPTYPENAME_H
#define TEMPLAT_UTIL_GETCPPTYPENAME_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/demangle.h"

namespace TempLat
{

  /** @brief A class which calls typeid and demangles it if possible.
   *
   *
   * Unit test: ctest -R test-getcpptypename
   **/
  class GetCPPTypeName
  {
  public:
    template <typename T> static inline std::string get(const T &instance) { return get<T>(); }

    template <typename T> static inline std::string get() { return Demangle::demangle(typeid(T).name()); }
  };

} // namespace TempLat

#endif
