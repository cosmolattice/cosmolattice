#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_FFTWMEMORYLAYOUT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_FFTWMEMORYLAYOUT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <algorithm>

#ifndef NOFFT
#ifndef NOMPI
#include "fftw3-mpi.h"
#endif
#endif

#ifndef NOFFT
#include "fftw3.h"
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/fftw/fftwplanner.h"
#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"

namespace TempLat {
    
    /** \brief Implements part of FFTLibraryInterface, computes the local memory associated to the global problem, for FFTW.
     *
     *
     * Unit test: make test-fftwmemorylayout
     **/
    
    class FFTWMemoryLayout : public FFTWPlanner {
    public:
        /* Put public methods here. These should change very little over time. */
        FFTWMemoryLayout() {  }
        
        virtual FFTLayoutStruct computeLocalSizes(MPICartesianGroup group,  std::vector<ptrdiff_t> nGridPoints, bool forbidTransposition = false) {
            FFTLayoutStruct result( nGridPoints, true, false);
            /* default: everything is local. */
            ptrdiff_t nDimensions = nGridPoints.size();

            std::vector<ptrdiff_t> confLocalSizes( nGridPoints);
            std::vector<ptrdiff_t> confLocalStarts(nDimensions, 0);
            std::vector<ptrdiff_t> fourLocalSizes(nGridPoints);
            std::vector<ptrdiff_t> fourLocalStarts(nDimensions, 0);
            std::vector<ptrdiff_t> fourTransposition(nDimensions);
            std::iota(fourTransposition.begin(), fourTransposition.end(), 0);
            
            fourLocalSizes.back() = fourLocalSizes.back() / 2 + 1;
            confLocalSizes.back() = 2 * fourLocalSizes.back();

            ptrdiff_t fftwRequiredMemory = 0;

#ifndef NOMPI
            if (nDimensions > 1 ) {
                std::vector<ptrdiff_t> globalLayout(result.fourierSpace.getLocalSizes().begin(), result.fourierSpace.getLocalSizes().end());

                bool doTranspose = nDimensions > 2 && group.size() > 1;

                doTranspose = doTranspose && ! forbidTransposition;

//say << "globalLayout: " << globalLayout << "\ninput: " << result << "\n";
                if ( doTranspose ) {

                    fftwRequiredMemory = fftw_mpi_local_size_transposed((int) nDimensions, globalLayout.data(), group.getComm(),
                                                   confLocalSizes.data(),
                                                   confLocalStarts.data(),
                                                   fourLocalSizes.data() + 1,
                                                   fourLocalStarts.data() + 1
                                                   );

                    std::swap( fourTransposition[0], fourTransposition[1] );

                } else {

                    fftwRequiredMemory = fftw_mpi_local_size((int) nDimensions, globalLayout.data(), group.getComm(),
                                                   fourLocalSizes.data(),
                                                   fourLocalStarts.data()
                                                   );
                    std::copy(fourLocalSizes.begin(), fourLocalSizes.end(), confLocalSizes.begin());
                    std::copy(fourLocalStarts.begin(), fourLocalStarts.end(), confLocalStarts.begin());
                    confLocalSizes.back() *= 2;
                    
                }

//say << "output:\n" << result << "\n";
            }
#endif

            result.configurationSpace.setLocalSizes(confLocalSizes);
            result.configurationSpace.setLocalStarts(confLocalStarts);
            result.fourierSpace.setLocalSizes(fourLocalSizes);
            result.fourierSpace.setLocalStarts(fourLocalStarts);
            result.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);

            result.addExternalMemoryRequest(2 * fftwRequiredMemory); /* fftwRequiredMemory is in unit of complex numbers, addExternalMemoryRequest expects units of real numbers. */
            
            result.fourierSpace.setHermitianPartners(FFTWHermitianPartners::create(result.configurationSpace.getGlobalSizes()));
            
            return result;
        };
        
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        
        
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}



#endif

