#ifndef TEMPLAT_FFT_TYPES_FFTMALLOCFREE_H
#define TEMPLAT_FFT_TYPES_FFTMALLOCFREE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019



#include <type_traits>


#ifndef NOMPI
// No need for PFFT at this level: use FFTW's allocation.
//#ifndef NOPPFT
//#include "pfft.h"
//#else
#include "fftw3-mpi.h"
#endif
//#endif
#include "fftw3.h"

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {
    
    /** \brief A class which selects the appropriate malloc and free functions for the lattice memory. Templated for double and float.
     * Preprocessor flags for FFTW, FFTW_MPI or PFFT.
     * If you want another memory alignment, just change it here globally. We can't be bothered to implement the templating
     * for the various allocators that the libraries all pass. In the end they all only care about the same thing, alignment of the
     * first entry with an x-sized block, for vectorization of the instructions. FFTW does a great job.
     *
     * Unit test: make test-fftmallocfree
     **/
    
    template <typename T>
    class FFTMallocFree {
    public:
        /* Put public methods here. These should change very little over time. */
        template <typename S = T>
        static typename std::enable_if< std::is_same<double, S>::value, void>::type * malloc(size_t n) {
            return fftw_malloc(n);
        }
        
#ifndef NOFFTFLOAT
        template <typename S = T>
        static typename std::enable_if< std::is_same<float, S>::value, void>::type * malloc(size_t n) {
            return fftwf_malloc(n);
        }
#endif

        static inline void free(void *p) {
            fftw_free(p);
        }
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        /** \brief private constructor: no one should instantiate this thing. */
        FFTMallocFree() {
            
        }
        
        
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/types/fftmallocfree_test.h"
#endif


#endif
