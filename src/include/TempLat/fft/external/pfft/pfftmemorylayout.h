#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_PFFTMEMORYLAYOUT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_PFFTMEMORYLAYOUT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#ifndef NOFFT
#ifndef NOMPI
#ifndef NOPPFT
#include "pfft.h"
#endif
#endif
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/external/pfft/pfftplanner.h"
#include "TempLat/fft/external/fftw/fftwhermitianpartners.h"

namespace TempLat {
    
    MakeException(PFFTMemoryLayoutException);
    
    /** \brief Implements part of FFTLibraryInterface, computes the local memory associated to the global problem, for PFFT. Build on this to replace PFFT.
     *
     * Unit test: make test-pfftmemorylayout
     **/
    
    class PFFTMemoryLayout : public PFFTPlanner {
    public:
        static bool PFFTWITHTRANSPOSITION() {
            return false;
        }

        /* Put public methods here. These should change very little over time. */
        PFFTMemoryLayout() { }
        
        
        virtual FFTLayoutStruct computeLocalSizes(MPICartesianGroup group, std::vector<ptrdiff_t> nGrid, bool forbidTransposition = PFFTWITHTRANSPOSITION()) {
            
            if (! forbidTransposition && ! PFFTWITHTRANSPOSITION() ) {
                forbidTransposition = true;
                if(group.getRank() == 0) say << KBLU "Sorry, currently only the un-transposed version of PFFT works. Forcing forbidTransposition = true now. This is slightly slower, but if FFTs are only done in the startup phase and an occasional measurement, they should not be the bottle neck. Want to work on this issue? See TempLat/lattice/field/integrationtesting/randomfield_test.h. That test fails if you enable transposed output in PFFT.\n" KRESET ;
            }

            ptrdiff_t nDimensions = nGrid.size();
            
            if ( group.getNumberOfDividedDimensions() >= nDimensions ) throw PFFTMemoryLayoutException("Inconsistent: the number of dimensions in which your Cartesian MPIGroup is split, is larger than / equal to the number of dimensions in the lattice:", group.getNumberOfDividedDimensions(), " >= ", nDimensions);
            
            FFTLayoutStruct result(nGrid, false, true);
            /* default: everything is local. */
            std::vector<ptrdiff_t> globalLayout(nGrid);
            
            std::vector<ptrdiff_t> confLocalSizes(nGrid);
            std::vector<ptrdiff_t> confLocalStarts(nDimensions, 0);
            std::vector<ptrdiff_t> fourLocalSizes(nGrid);
            std::vector<ptrdiff_t> fourLocalStarts(nDimensions, 0);
            std::vector<ptrdiff_t> fourTransposition(nDimensions);
            std::iota(fourTransposition.begin(), fourTransposition.end(), 0);
            
            fourLocalSizes.back() = fourLocalSizes.back() / 2 + 1;
            confLocalSizes.back() = 2 * fourLocalSizes.back();
            
            ptrdiff_t pfftRequiredMemory = 0;

            if (nDimensions > 1 ) {
                
                bool doTranspose = nDimensions > 2 && group.size() > 1;
                
                doTranspose = doTranspose && ! forbidTransposition;

                unsigned int flags = PFFT_PADDED_R2C;
                if ( doTranspose ) {
                    flags |= PFFT_TRANSPOSED_OUT;
                }
                //                say << "globalLayout: " << globalLayout << "\ninput: " << result << "\n";
                
                pfftRequiredMemory = pfft_local_size_dft_r2c(
                                        (int) nDimensions,
                                        globalLayout.data(),
                                        group.getComm_onlyDividedDimensions(), /* crucial this, for pfft. Only split MPI group in the dimensions in which we have more than 1 owners. */
                                        flags,
                                        confLocalSizes.data(),
                                        confLocalStarts.data(),
                                        fourLocalSizes.data(),
                                        fourLocalStarts.data()
                                        );
                
                if ( doTranspose ) {
                    
                    /* as opposed to FFTW, PFFT *does* transpose the sizes and starts, in this routine. So
                     * for consistency, since we use the transitionMap, undo the transposition in size and start arrays.
                     *
                     * Moreover, read the docs of PFFT carefully. The transposition is as follows:
                     *
                     * configuration space: ( ni / Pi denotes dimension i split over Pi blocks)
                     * n0 / P0 x n1 / P1 x n2 / P2 x n3 x n4 ... x nN
                     *
                     * to fourier space:
                     * n1 / P0 x n2 / P1 x n3 / P2 x n0 x n4 ... x nN
                     *
                     * that is: n0 goes to the first unsplit dimension.
                     */
                    
                    ptrdiff_t nSplit = group.getNumberOfDividedDimensions();
                    if ( nSplit >= nDimensions ) nSplit = nDimensions - 1;

                    for ( ptrdiff_t i = 0; i < nSplit; ++i) {
                        fourTransposition[i] = i + 1;
                    }
                    fourTransposition[nSplit] = 0;

                    say << "Transposition computed: " << fourTransposition << "\n";
                }
                
            } else {
                /* We manually need to set the size of the last dimension to the r2c setup: 2 * (N/2 + 1). */
                /* done in constructor of FFTLayoutStruct */
            }
            
            result.configurationSpace.setLocalSizes(confLocalSizes);
            result.configurationSpace.setLocalStarts(confLocalStarts);
            result.fourierSpace.setLocalSizes(fourLocalSizes);
            result.fourierSpace.setLocalStarts(fourLocalStarts);
            result.fourierSpace.setTranspositionMap_memoryToGlobalSpace(fourTransposition);
            
            result.addExternalMemoryRequest(2 * pfftRequiredMemory); /* pfftRequiredMemory is in unit of complex numbers, addExternalMemoryRequest expects units of real numbers. */
 
            result.fourierSpace.setHermitianPartners(FFTWHermitianPartners::create(result.configurationSpace.getGlobalSizes()));
            
            //            say << "output: " << result << "\n";
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

