#ifndef TEMPLAT_FFT_FFTMPIDOMAINSPLIT_H
#define TEMPLAT_FFT_FFTMPIDOMAINSPLIT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

namespace TempLat {

    /** \brief A class which combines the MPIDomainSplit with the FFTLibrarySelector limits on domain splitting.
     *
     * Unit test: make test-fftmpidomainsplit
     **/

    class FFTMPIDomainSplit {
    public:
    /* Put public methods here. These should change very little over time. */

        static std::vector<int> makeDomainDecomposition(ptrdiff_t groupSize, ptrdiff_t nDimensions) {

            
            ptrdiff_t nDimensionsToSplit = FFTLibrarySelector::getMaximumNumberOfDimensionsToDivide(nDimensions);
            
            MPIDomainSplit theSplit(groupSize, nDimensions, nDimensionsToSplit);
            
            return theSplit;
        }

        static MPICartesianGroup makeMPIGroup(MPICommReference baseGroup, ptrdiff_t nDimensions) {
            return MPICartesianGroup(baseGroup, nDimensions, makeDomainDecomposition(baseGroup.size(), nDimensions));
        }
        /* default using comm_world */
        static MPICartesianGroup makeMPIGroup(ptrdiff_t nDimensions) {
            return makeMPIGroup(MPICommReference(), nDimensions);
        }
    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        FFTMPIDomainSplit() {

        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/fftlibraryselector_test.h"
#endif
#ifdef TEMPLATTEST
#include "TempLat/fft/fftmpidomainsplit_test.h"
#endif


#endif
