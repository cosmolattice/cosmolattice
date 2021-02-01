#ifndef TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_H
#define TEMPLAT_UTIL_RANDOM_RANDOMGAUSSIAN_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/random/randomuniform.h"

namespace TempLat {

    MakeException(RandomGaussianWrongCallOrderException);

    /** \brief An extra namespace to avoid confusion with random gaussian fields. */

    namespace Util {

        /** \brief A gaussian random variable. Is aware of its own state and counts the number of values that have been returned.
         *
         * Unit test: make test-randomgaussian
         **/

        class RandomGaussian {
        public:
            /* Put public methods here. These should change very little over time. */
            RandomGaussian(std::string seed) :
            mRandomUniform(seed),
            mStateCounter(0u),
            mHaveCachedValue(false)
            {

            }

            void rebase() {
                mRandomUniform.rebase();
            }

            const size_t& getState() const {
                return mStateCounter;
            }

            const auto getSeed() const {
                return mRandomUniform.getSeed();
            }

            double operator()() {
                return getNextGaussian();
            }

            std::array<double, 2> getNextPair(bool real =false, bool unitary = false) {//Even if this is not completely consistent with the name, it is convenient to be able to use this class to generate numbers with a real gaussian distribution or uniformly on the unit disk.
                return getNextGaussianPair(true, real, unitary);
            }

            friend
            std::ostream& operator<< ( std::ostream& ostream, const RandomGaussian& pr) {
                ostream << "RandomGaussian - seed string: \"" << pr.mRandomUniform.getSeedString() << "\" - seed value: " << pr.mRandomUniform.getSeed() << " - number of values fetched: " << pr.getState();
                return ostream;
            }

        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */
            RandomUniform<> mRandomUniform;
            size_t mStateCounter;
            bool mHaveCachedValue;
            double mCachedValue;

            static constexpr double cTwoPi = 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359642961730265646132941876892;

            double getNextGaussian() {
                double result = 0.;
                if( mHaveCachedValue ) {
                    mHaveCachedValue = false;
                    result = mCachedValue;
                } else {

                    /* We update the state counter by one, and update by one
                     again when the cached value is fetched. So the pair
                     computation method should not update the state counter. */
                    auto pair = getNextGaussianPair(false);

                    result = pair[0];
                    mCachedValue = pair[1];

                    mHaveCachedValue = true;
                }
                ++mStateCounter;
                return result;
            }

            std::array<double, 2u> getNextGaussianPair(bool updateStateCounter = true, bool real = false, bool unitary = false) {//Even if this is not completely consistent with the name, it is convenient to be able to use this class to generate numbers with a real gaussian distribution or uniformly on the unit disk.
                if ( mStateCounter % 2 ) throw RandomGaussianWrongCallOrderException("Cannot call getNextGaussianPair after odd number of calls to getNextGaussian(). This breaks reproducibility." );

                if ( updateStateCounter ) mStateCounter += 2;

                double r0 = mRandomUniform();
                double r1 = mRandomUniform();
                
                double boxMullerR = r0 == 0 ? std::numeric_limits<double>::max() : std::sqrt( -2 * std::log(r0) );
                double boxMullerTheta = cTwoPi * r1;

                if(real) boxMullerTheta = 0;
                if(unitary) boxMullerR = 1;


                return std::array<double, 2> {{
                    boxMullerR * std::cos(boxMullerTheta),
                    boxMullerR * std::sin(boxMullerTheta)
                }};
            }

        public:
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };

    }
}

#ifdef TEMPLATTEST
#include "TempLat/util/random/randomgaussian_test.h"
#endif


#endif
