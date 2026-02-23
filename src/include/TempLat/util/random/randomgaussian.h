#ifndef TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_H
#define TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include <iomanip>
#include <sstream>

#include "TempLat/util/random/randomuniform.h"

namespace TempLat
{
  MakeException(RandomGaussianWrongCallOrderException);
  /** @brief A gaussian random variable. Is aware of its own state and counts the number of values that have been
   *returned.
   *
   * Unit test: ctest -R test-randomgaussian
   **/
  class RandomGaussian
  {
    using INT2 = typename RandomUniform<>::IntegerType;

  public:
    // Put public methods here. These should change very little over time.

    using IntegerType = INT2;

    RandomGaussian(const std::string &seed) : mRandomUniform(seed) {}

    auto getSeed() const { return mRandomUniform.getSeed(); }
    auto getSeedString() const { return mRandomUniform.getSeedString(); }

    DEVICE_FORCEINLINE_FUNCTION
    auto getPair(INT2 r, INT2 c, INT2 g, bool real = false, bool unitary = false) const
    { // Even if this is not completely consistent with the name, it is convenient to be able to use this class to
      // generate numbers with a real gaussian distribution or uniformly on the unit disk.
      return getNextGaussianPair(r, c, g, real, unitary);
    }

    DEVICE_FORCEINLINE_FUNCTION
    double get(INT2 r, INT2 c, INT2 g) const { return getPair(r, c, g, false, false)[0]; }

    friend std::ostream &operator<<(std::ostream &ostream, const RandomGaussian &pr)
    {
      ostream << "RandomGaussian - seed string: \"" << pr.mRandomUniform.getSeedString()
              << "\" - seed value: " << pr.mRandomUniform.getSeed();
      return ostream;
    }

    /**
     * @brief Serializes the complete Gaussian RNG state to a string
     * @return String containing underlying uniform RNG state, counter, and Box-Muller cache
     */
    std::string saveState() const { return mRandomUniform.saveState(); }

    /**
     * @brief Restores Gaussian RNG state from a serialized string
     * @param state String produced by saveState()
     */
    void loadState(const std::string &state) { mRandomUniform.loadState(state); }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    RandomUniform<> mRandomUniform;

    static constexpr double cTwoPi =
        6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359642961730265646132941876892;

    DEVICE_FORCEINLINE_FUNCTION
    device::array<double, 2u> getNextGaussianPair(INT2 r, INT2 c, INT2 g, bool real = false, bool unitary = false) const
    { // Even if this is not completely consistent with the name, it is convenient to be able to use this class to
      // generate numbers with a real gaussian distribution or uniformly on the unit disk.

      const auto result = mRandomUniform.getPair(r, c, g);

      const double &r0 = result[0];
      const double &r1 = result[1];

      const double boxMullerR =
          unitary ? 1 : (r0 == 0 ? std::numeric_limits<double>::max() : device::sqrt(-2 * device::log(r0)));
      const double boxMullerTheta = real ? 0 : cTwoPi * r1;

      return device::array<double, 2u>{boxMullerR * device::cos(boxMullerTheta), boxMullerR * device::sin(boxMullerTheta)};
    }
  };
} // namespace TempLat

#endif
