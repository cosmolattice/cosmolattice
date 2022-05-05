#ifndef TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_H
#define TEMPLAT_LATTICE_ALGEBRA_RANDOM_RANDOMGAUSSIANFIELD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <complex>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/random/randomgaussian.h"
#include "TempLat/lattice/algebra/coordinates/dimensioncountrecorder.h"

namespace TempLat {

    MakeException(RandomGaussianFieldNegativeFrequencyException);

    /** \brief A class which initializes your complex random gaussian field. ONLY WORKS FOR FFTW R2C complex layouts. It has a state, only call for monotonically increasing
     *  last dimension, and walk an entire rod in the last dimension
     *  before moving sideways. Otherwise this will be freaking slow.
     *  This field is aware of wavenumbers: same layout is repeated
     *  for different resolutions. Increasing the resolution with the
     *  same random seed, means that you keep the same structure that
     *  you had at the lower resolution, with additional high frequency
     *  randomness.
     *
     *  And yet, the current implementation is slow: on each call to get
     *  we allocate a new std::vector, because we now take into account
     *  that the gaussian random field must have hermitian symmetry.
     *  If we didn't do this, FFTW would have no problem and still
     *  produce a real field with C2R and a hermitian symmetric
     *  field with a consequential R2C, but this field then would not
     *  be exactly equal to the input. Just for consistency, we chose to
     *  sacrifice the speed and provide an input which has exactly
     *  the hermitian symmetry which FFTW would otherwise impose.
     *
     * Unit test: make test-randomgaussianfield
     **/
    template <typename T, bool Real, bool Unitary>
    class RandomGaussianFieldHelper : public DimensionCountRecorder {
    public:
        /* Put public methods here. These should change very little over time. */
        RandomGaussianFieldHelper(std::string baseSeed,std::shared_ptr<MemoryToolBox> pToolBox) :
        DimensionCountRecorder(SpaceStateInterface::SpaceType::undefined),
        mBaseSeed(baseSeed),
        prng(baseSeed),
        mToolBox(pToolBox)
        {
          confirmSpace(mToolBox->mLayouts.getFourierSpaceLayout(), SpaceStateInterface::SpaceType::Fourier);
        }


        std::complex<T> get(const int & i) {
            return get(mToolBox->getCoordFourier(i)); //Coded like that for testing purposes.
        }


        std::complex<T> get(std::vector<ptrdiff_t> coord) {

            std::vector<ptrdiff_t> hermitianPartner;

            auto hermitianType = getCurrentLayout()
            .getHermitianPartners()
            ->putHermitianPartner(
                                  coord,
                                  hermitianPartner
                                  );

            updatePRNG(hermitianPartner);

            auto pair = prng.getNextPair( Real, Unitary);
            /* ordered the if-statement by most-occurring case first. */
            return std::complex<T> (pair[0],
                                       hermitianType == HermitianRedundancy::none
                                    || hermitianType == HermitianRedundancy::positivePartner
                                    ?
                                    pair[1]
                                    :
                                    hermitianType == HermitianRedundancy::negativePartner
                                    ?
                                    - pair[1]
                                    :
                                    T(0)
                                  );
        }



        std::string toString() const {
            return "Random gaussian field with seed: \"" + mBaseSeed + "\"";
        }

        const auto getCurrentSeed() const {
            return prng.getSeed();
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::string mBaseSeed;
        std::vector<ptrdiff_t> rodPosition;
        Util::RandomGaussian prng;
        std::shared_ptr<MemoryToolBox> mToolBox;

        /** \brief Verifies that the coordinates asked for are
         *  on the same rod that we are in, and that the last dimension
         *  is monotonically growing.
         *  If not, rebuild the prng.
         */
        void updatePRNG(const std::vector<ptrdiff_t> &coordinates) {
            ptrdiff_t nd = getNDimensions();
            bool needUpdate = (ptrdiff_t) rodPosition.size() < nd;

            /* can only monotonically grow. Are we going down? Restart from zero. */
            needUpdate = needUpdate || rodPosition.back() > coordinates[nd - 1];
            if ( needUpdate ) rodPosition.resize(nd);
            for (ptrdiff_t i = 0; i < nd - 1 ; ++i ) {
                needUpdate = needUpdate || rodPosition[i] != coordinates[i];
                rodPosition[i] = coordinates[i];
            }
            if ( needUpdate ) {
                std::string rodPositionString = "";
                for (ptrdiff_t i = 0; i < nd - 1; ++i) {
                    rodPositionString += std::to_string(rodPosition[i]) + ", ";
                }
                prng = Util::RandomGaussian(mBaseSeed + " " + rodPositionString);
                rodPosition.back() = 0;
            }

            /* next, update the prng to be at one step before coordinates[nd - 1] */
            ptrdiff_t goal = coordinates[nd - 1] ;

            if ( goal < 0 ) throw RandomGaussianFieldNegativeFrequencyException("Random gaussian field assumes a positive wavenumber-only layout of the rods, exactly as in FFTW's R2C transforms. Sorry, you asked for a negative frequency.");

            while (((ptrdiff_t)prng.getState() / 2) < goal) prng.getNextPair();

            rodPosition.back() = coordinates[nd - 1];
        }

    };

    class RandomGaussianFieldTester{
        public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename T>
    using RandomGaussianField = RandomGaussianFieldHelper<T,false,false>;

    template<typename T>
    using RandomRayleighField = RandomGaussianFieldHelper<T,true,false>;

    template<typename T>
    using RandomUniformUnitaryField = RandomGaussianFieldHelper<T,false,true>;

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/random/randomgaussianfield_test.h"
#endif


#endif
