#ifndef TEMPLAT_UTIL_ISINCONTAINER_H
#define TEMPLAT_UTIL_ISINCONTAINER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include <algorithm>
#include <iterator>

namespace TempLat
{

  /** @brief A class which checks wheter container contains some objects. Need to be std compatible.
   *https://stackoverflow.com/a/19299611
   *
   *
   * Unit test: ctest -R test-isincontainer
   **/
  class IsInContainer
  {
  public:
    // Put public methods here. These should change very little over time.
    IsInContainer() = delete;

    template <typename T, typename R> static bool check(T &&t, R &&r)
    {
      return std::find(std::begin(r), std::end(r), t) != std::end(r);
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
  };

} // namespace TempLat

#endif
