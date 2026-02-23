#ifndef TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_H
#define TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg, Franz R. Sattler,  Year: 2025

#include <cstdint>
#include <random>
#include <sstream>

#include "TempLat/util/hash/keccakhash.h"
#include "TempLat/parallel/device.h"
#include "TempLat/parallel/device_memory.h"

#include <Random123/philox.h>

namespace TempLat
{
  /** @brief A class which gives pseudo random sequences, based on a string random seed, stable across platforms.
   * Implementation note: the C++14 standard guarantees cross-platform stability of Mersenne-Twister std::mt19937_64.
   * So we use that. Note: not sure about the other random generator.
   *
   * Unit test: ctest -R test-randomuniform
   **/
  template <typename dummy = void> class RandomUniform
  {
    using RNG2 = r123::Philox2x32;
    using INT2 = typename RNG2::ctr_type::value_type;

  public:
    // Put public methods here. These should change very little over time.

    using IntegerType = INT2;

    RandomUniform(const std::string &stringSeed)
        : mStringSeed(stringSeed), mHashSeed(KeccakHash::compute(stringSeed)),
          mSeed(static_cast<INT2>((uint64_t)mHashSeed))
    {
    }

    /**
     * @brief Serializes the complete RNG state to a string
     * @return String containing serialized mt19937_64 state and counter
     */
    std::string saveState() const
    {
      std::ostringstream oss;
      oss << *mStringSeed;
      return oss.str();
    }

    /**
     * @brief Restores RNG state from a serialized string
     * @param state String produced by saveState()
     */
    void loadState(const std::string &state)
    {
      std::istringstream iss(state);
      iss >> *mStringSeed;
      mHashSeed = KeccakHash::compute(*mStringSeed);
      mSeed = static_cast<INT2>((uint64_t)mHashSeed);
    }

    auto getSeed() const { return mSeed; }

    const std::string &getSeedString() const { return *mStringSeed; }

    DEVICE_FORCEINLINE_FUNCTION
    double get(INT2 r, INT2 c, INT2 gen) const { return getPair(r, c, gen)[0]; }

    DEVICE_FORCEINLINE_FUNCTION
    device::array<double, 2> getPair(INT2 r, INT2 c, INT2 gen) const
    {
      const RNG2 rng;

      // create a counter and key for the generator
      const RNG2::ctr_type counter = {{r, c}};
      const RNG2::key_type key = {{mSeed + gen}};
      // draw a pair of numbers
      const RNG2::ctr_type result = rng(counter, key);

      return {{integer_to_double(result[0]), integer_to_double(result[1])}};
    }

    template <typename T> DEVICE_FORCEINLINE_FUNCTION double integer_to_double(T value) const
    {
      return (static_cast<double>(value) - static_cast<double>(std::numeric_limits<T>::min())) /
             static_cast<double>(std::numeric_limits<T>::max());
    }

    /** @brief For testing purposes, we need to compare prng's, specifically their seeds. */
    friend bool operator==(const RandomUniform &a, const RandomUniform &b) { return a.getSeed() == b.getSeed(); }

    friend std::ostream &operator<<(std::ostream &ostream, const RandomUniform &pr)
    {
      ostream << "RandomUniform - seed string: \"" << pr.getSeedString() << "\" - seed value: " << pr.getSeed() << "\n";
      return ostream;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    device::memory::host_string mStringSeed;
    KeccakHash::ResultType mHashSeed;
    INT2 mSeed;
  };
} // namespace TempLat

#endif
