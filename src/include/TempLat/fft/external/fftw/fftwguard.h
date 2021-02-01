#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWGUARD_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWGUARD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#ifndef NOMPI
#include <mpi.h>
#endif

#ifndef NOFFT
#ifndef NOMPI
#include "fftw3-mpi.h"
#endif
#endif

#ifndef NOFFT
#include "fftw3.h"
#endif

#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/threads/threadsettings.h"


namespace TempLat {
    
    /** \brief A class which guards intialization and finalization of FFTW.
     * Only used by MPIGuard, which manually calls its destructor.
     *
     *
     * Unit test: make test-fftwguard
     **/
    
    class FFTWGuard : public FFTLibraryInterface::SessionGuard {
    public:
        /* Put public methods here. These should change very little over time. */

        FFTWGuard(bool verbose = true) : mVerbose (verbose)  {
#ifndef NOFFT
            if (mVerbose ) sayShort << "Calling FFTW local initializations.\n";
            fftw_init_threads();
#ifndef NOFFTFLOAT
            fftwf_init_threads();
#endif
            /* set the default: */

            if (mVerbose ) sayShort << "Calling FFTW thread initializations.\n";
            fftw_plan_with_nthreads(ThreadSettings::getMaxThreadCount());
#ifndef NOFFTFLOAT
            fftwf_plan_with_nthreads(ThreadSettings::getMaxThreadCount());
#endif

#ifndef NOMPI
#ifdef NOPFFT

            if (mVerbose ) sayShort << "Calling FFTW MPI initializations.\n";

            /* this also suffices for pfft, but we keep both lines in case in the future pfft decides to need its own init. */
            fftw_mpi_init();
#ifndef NOFFTFLOAT
            fftwf_mpi_init();
#endif

#endif
#endif
            
#endif
            
        }
        
        ~FFTWGuard() {
#ifndef NOFFT
            
            if (mVerbose ) sayShort << "Calling FFTW thread finalizations.\n";
            fftw_cleanup_threads();
#ifndef NOFFTFLOAT
            fftwf_cleanup_threads();
#endif

            if (mVerbose ) sayShort << "Calling FFTW local finalizations.\n";
            fftw_cleanup();
#ifndef NOFFTFLOAT
            fftwf_cleanup();
#endif

#ifndef NOMPI
#ifdef NOPFFT
            /* this also suffices for pfft, but we keep both lines in case in the future pfft decides to need its own init. */
            if (mVerbose ) sayShort << "Calling FFTW MPI finalizations.\n";
            fftw_mpi_cleanup();
#ifndef NOFFTFLOAT
            fftwf_mpi_cleanup();
#endif

#endif
#endif
            
#endif
        }

        /* delete the copy constructor and copy assignment */
        FFTWGuard(const FFTWGuard& other) = delete;
        FFTWGuard& operator=(const FFTWGuard& other) = delete;
        FFTWGuard& operator=(FFTWGuard&& other) = delete;

        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        bool mVerbose;

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwguard_test.h"
#endif


#endif
