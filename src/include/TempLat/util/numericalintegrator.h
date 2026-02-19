#ifndef TEMPLAT_UTIL_NUMERICALINTEGRATOR_H
#define TEMPLAT_UTIL_NUMERICALINTEGRATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include <vector>

namespace TempLat
{
  /** @brief A class which encapsulate some newto-cotes integration formula. Assume that the point are equispaced.
   *
   *
   * Unit test: ctest -R test-numericalintegrator
   **/
  class NumericalIntegrator
  {
  public:
    // Put public methods here. These should change very little over time.
    NumericalIntegrator() {}

    template <typename T> static T integrate(const std::vector<T> &vec, T dt)
    {
      T res = 0;
      if (vec.size() == 3) {
        res = dt / 3.0 * (vec[0] + 4 * vec[1] + vec[2]);
      }
      return res;
    }
  };

  template <typename T> T integrate(const std::vector<T> &vec, T dt) { return NumericalIntegrator::integrate(vec, dt); }

} // namespace TempLat

#endif
