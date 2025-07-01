#ifndef TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_H
#define TEMPLAT_UTIL_RANDOM_RANDOMUNIFORM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <Kokkos_Core_fwd.hpp>
#include <cstdint>
#include <random>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/hash/keccakhash.h"
#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat
{
  /** \brief A class which gives pseudo random sequences, based on a string random seed, stable across platforms.
   * Implementation note: the C++14 standard guarantees cross-platform stability of Mersenne-Twister std::mt19937_64.
   * So we use that. Note: not sure about the other random generator.
   *
   * Unit test: make test-randomuniform
   **/
#ifndef NOKOKKOS
  template <typename dummy = void> class RandomUniform
  {
  public:
    /* Put public methods here. These should change very little over time. */
    RandomUniform(std::string stringSeed) : RandomUniform(stringSeed, Kokkos::DefaultExecutionSpace().concurrency()) {}

    RandomUniform(std::string stringSeed, size_t poolSize)
        : mStringSeed(stringSeed), mHashSeed(KeccakHash::compute(mStringSeed)), mSeed(mHashSeed), mPoolSize(poolSize),
          mRandomPool(mSeed)
    {
      rebase();
    }

    void rebase() { mRandomPool.init(mSeed, mPoolSize); }

    KOKKOS_FORCEINLINE_FUNCTION
    size_t getPoolSize() const { return mPoolSize; }

    const std::string &getSeedString() const { return mStringSeed; }

    const auto getSeed() const { return mSeed; }

    KOKKOS_FORCEINLINE_FUNCTION
    double operator()(size_t state) const
    {
      // obtain a generator from the pool
      auto generator = mRandomPool.get_state(state);
      // draw a number
      const double x = generator.drand(0., 1.);
      // do not forget to release the state of the engine
      mRandomPool.free_state(generator);
      return x;
    }

    KOKKOS_FORCEINLINE_FUNCTION
    double operator()() const
    {
      // obtain a generator from the pool
      auto generator = mRandomPool.get_state();
      // draw a number
      const double x = generator.drand(0., 1.);
      // do not forget to release the state of the engine
      mRandomPool.free_state(generator);
      return x;
    }

    /** \brief For testing purposes, we need to compare prng's, specifically their seeds. */
    friend bool operator==(const RandomUniform &a, const RandomUniform &b) { return a.getSeed() == b.getSeed(); }

    friend std::ostream &operator<<(std::ostream &ostream, const RandomUniform &pr)
    {
      ostream << "RandomUniform - seed string: \"" << pr.getSeedString() << "\" - seed value: " << pr.getSeed() << "\n";
      return ostream;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string mStringSeed;
    KeccakHash::ResultType mHashSeed;
    uint64_t mSeed;
    size_t mPoolSize;
    Kokkos::Random_XorShift64_Pool<> mRandomPool;
  };
#else
  template <typename RandomGenerator = std::mt19937_64> class RandomUniform
  {
  public:
    /* Put public methods here. These should change very little over time. */
    RandomUniform(std::string stringSeed)
        : mStringSeed(stringSeed), mHashSeed(KeccakHash::compute(mStringSeed)), mUIntSeed(mHashSeed), mSeed(mUIntSeed),
          mMin(RandomGenerator::min()), /* Double! Yes! No trouble with subtracting unsigned's. */
          mMax(RandomGenerator::max()), /* Double! Yes! No trouble with subtracting unsigned's. */
          mRange(mMax - mMin),          /* Double! Yes! No trouble with subtracting unsigned's. */
          mStateCounter(0u)
    {
      rebase();
    }

    void rebase()
    {
      mMersenneTwister.seed(mSeed);
      mStateCounter = 0u;
    }

    const std::string &getSeedString() const { return mStringSeed; }

    const typename RandomGenerator::result_type &getSeed() const { return mSeed; }

    const size_t &getState() const { return mStateCounter; }

    typename RandomGenerator::result_type getNextBareUInt()
    {

      ++mStateCounter;

      return mMersenneTwister();
    }

    double operator()() { return scaleUIntToDouble(getNextBareUInt()); }

    /** \brief For testing purposes, we need to compare prng's, specifically their seeds. */
    friend bool operator==(const RandomUniform &a, const RandomUniform &b) { return a.getSeed() == b.getSeed(); }

    friend std::ostream &operator<<(std::ostream &ostream, const RandomUniform &pr)
    {
      ostream << "RandomUniform - seed string: \"" << pr.getSeedString() << "\" - seed value: " << pr.getSeed()
              << " - number of values fetched: " << pr.getState();
      return ostream;
    }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    std::string mStringSeed;
    KeccakHash::ResultType mHashSeed;
    uint64_t mUIntSeed;
    typename RandomGenerator::result_type mSeed;
    double mMin, mMax, mRange; /* Double! Yes! No trouble with subtracting unsigned's. */
    RandomGenerator mMersenneTwister;
    size_t mStateCounter;

    double scaleUIntToDouble(typename RandomGenerator::result_type value) { return (((double)value) - mMin) / mRange; }
  };
#endif

  struct RandomUniformTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };
} // namespace TempLat

#ifdef TEMPLATTEST
#include "TempLat/util/random/randomuniform_test.h"
#endif

#endif
