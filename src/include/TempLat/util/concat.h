#ifndef TEMPLAT_UTIL_CONCAT_H
#define TEMPLAT_UTIL_CONCAT_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

namespace TempLat
{

  /** @brief A class which concatenates lists.
   *
   * Unit test: ctest -R test-concat
   **/
  template <class S1, class S2> struct Concat;
  template <class S1, class S2> using Concat_t = typename Concat<S1, S2>::type;

} // namespace TempLat

#endif
