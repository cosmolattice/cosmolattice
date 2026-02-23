#ifndef TEMPLAT_UTIL_CONSTANTS_H
#define TEMPLAT_UTIL_CONSTANTS_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include <numbers>
#include "TempLat/parallel/device.h"

namespace TempLat
{
  /** @brief A class which
   * Class to store your constant. Some basic constants here, inherit from here and make a new class
   * in your project if you want more.
   *
   *
   * Unit test: ctest -R test-constants
   **/
  namespace Constants
  {

    template <typename T = double>
    constexpr T pi = std::numbers::pi_v<T>;

    // constexpr static T reducedMPlanck = 2.435363e18; //GEV2, PDG value.
    template <typename T = double> constexpr T reducedMPlanck = T(2.435e18); // Agreement between PDG and CODATA

    template <typename T = double> constexpr complex<T> I = complex<T>(0, 1);

    const std::string defaultString = "\u2716";

    // For convenience, redundantly define planck mass again, in float and double. can use whatever you want that way

    constexpr double MPl = 2.435e18;
    constexpr float fMPl = 2.435e18;

  }; // namespace Constants

} // namespace TempLat

#endif
