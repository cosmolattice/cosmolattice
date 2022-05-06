#ifndef TEMPLAT_LATTICE_MEASUREMENTS_TWOPOINTCORRELATOR_H
#define TEMPLAT_LATTICE_MEASUREMENTS_TWOPOINTCORRELATOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/measuringtools/toolwithownmemory.h"
#include "TempLat/lattice/measuringtools/radialprojector.h"
#include "TempLat/lattice/algebra/helpers/gettoolbox.h"

namespace TempLat {

    /** \brief A class which computes the two-point correlation == FFT::C2R(|f_k|^2) of
     *   whatever getter you give as input, then projects onto radius
     *   to get a function of distance only.
     *
     *
     * Unit test: make test-twopointcorrelator
     **/
    template <typename T>
    class TwoPointCorrelatorBuilder : public ToolWithOwnMemory<T> {
        using typename ToolWithOwnMemory<T>::fieldType;
        using ToolWithOwnMemory<T>::getInstance;
        using ToolWithOwnMemory<T>::getFieldForMeasurement;
    public:
        /* Put public methods here. These should change very little over time. */
        TwoPointCorrelatorBuilder(T instance,std::shared_ptr<MemoryToolBox> pToolBox) :
        ToolWithOwnMemory<T>(instance),
        mToolBox(pToolBox)
        {

        }

        auto measure(ptrdiff_t nLinearBins = -1) {
            fieldType temporary(getFieldForMeasurement("<f(x)f(x + r)>"));

            /* copy the configuration space values: */
            temporary = getInstance();

            /* this does FFT R2C */
            temporary.inFourierSpace() = pow<2>(abs(temporary.inFourierSpace()));

            /* project radially in configuration space */
            /* this does FFT C2R */
            auto projector = projectRadially(temporary, SpaceStateInterface::SpaceType::Configuration,mToolBox, false, false);

            return projector.measure(nLinearBins);
        }


    private:
      std::shared_ptr<MemoryToolBox> mToolBox;
        /* Put all member variables and private methods here. These may change arbitrarily. */

    };

    struct TwoPointCorrelatorTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    /** \brief The object to call, PowerSpectrum(yourObject) returns a newly constructed PowerSpectrumBuilder<T>,
     *  on which you can call PowerSpectrumBuilder<T>#measure to get your power spectrum.
     */
    template <typename T>
    auto TwoPointCorrelator(T instance) {
        return TwoPointCorrelatorBuilder<T>(instance,GetToolBox::get(instance));
    }

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/twopointcorrelator_test.h"
#endif


#endif
