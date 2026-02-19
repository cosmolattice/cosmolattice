#ifndef TEMPLAT_UTIL_FUNCTION_H
#define TEMPLAT_UTIL_FUNCTION_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

namespace TempLat
{

  /** @brief A macro to simplify writing lambda function. Don't use it in TempLat, for the interfaces.
   *
   *
   * Unit test: ctest -R test-function
   **/
#define Function(x, expr) [&](auto x) { return expr; }
} // namespace TempLat

#endif
