#ifndef TEMPLAT_FFT_FFTLIBRARYINTERFACE_H
#define TEMPLAT_FFT_FFTLIBRARYINTERFACE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/lattice/memory/memorylayouts/fftlayoutstruct.h"
#include "TempLat/lattice/memory/memoryblock.h"

namespace TempLat {

    /** \brief A pure abstract class (interface!) which defines the methods that you must implement for your new fft library to play well with us.
     *
     * Unit test: make test-fftlibraryinterface
     **/

    class FFTLibraryInterface {
    public:
    /* Put public methods here. These should change very little over time. */
        FFTLibraryInterface() { }
        virtual ~FFTLibraryInterface() { }

        /** \brief Return the number of dimensions over which the MPI processes can be distributed, given a problem of dimension nDimensions.
         *  FFTW can only divide in 1 dimension: the leading dimension.
         *  PFFT can divide an N dimensional setup over the leading N - 1 dimensions.
         *
         *  The calling function needs to know, because the user may switch between layouts after compilation.
         */
        virtual ptrdiff_t getMaximumNumberOfDimensionsToDivide(ptrdiff_t nDimensions) = 0;

        /** \brief The result holder for getIntrinsicRescaleToGetUnnormalizedFFT:
         *  two labeled double values.
         */
        struct IntrinsicScales {
            IntrinsicScales() : c2r(1), r2c(1) { }
            double c2r;
            double r2c;
            bool isDefault() { return c2r == 1 && r2c == 1; }
        };
        /** \brief The lattice objects expect an *unnormalized* FFT, such as FFTW and PFFT give:
         *  applying once forward and then backward, should return the input values multiplied by
         *  nGridPoints^nDimensions, i.e. the total number of values in the problem.
         *  If your library does not respect that, return the factor by which we must multiply the output
         *  values to obtain the unnormalized FFT. Return default IntrinsicScales (exact (double) 1)
         *  in order not to waste time,
         *  if your library does like FFTW and PFFT.
         */
        virtual IntrinsicScales getIntrinsicRescaleToGetUnnormalizedFFT(ptrdiff_t nDimensions, ptrdiff_t nGridPoints) = 0;

        /** \brief given an actual setup, return the description of the subarray of the global problem that this process holds. */
        virtual FFTLayoutStruct computeLocalSizes(MPICartesianGroup group,  std::vector<ptrdiff_t> nGridPoints, bool forbidTransposition = false) = 0;

        /** \brief If your library has different levels of patience for the planning phase, set it here.
         */
        virtual void setPlannerPatience(int level) = 0;

        /** \brief Yes, for once a nested class. The interface for your to-be-implemented FFT plan, but forward and backward.
         *  Complex-to-real and real-to-complex.
         *
         *  Your implementation of PlanInterface must take care of freeing the plan(s) upon destruction. So you should use shared_ptr's!
         */
        template <typename T>
        class PlanInterface {
        public:
            /* virtual desctructor can not be abstract: https://stackoverflow.com/a/13444839/2295722 */
            virtual ~PlanInterface() {};
            virtual void c2r(MemoryBlock<T> &mBlock) = 0;
            virtual void r2c(MemoryBlock<T> &mBlock) = 0;
        };

        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's.
         *  Since we use virtual methods here, we cannot use templates. Only one type of dynamic typing allowed by C++, either runtime (virtual) or compile time (template).
         */
        virtual std::shared_ptr<PlanInterface<float>> getPlans_float(const MPICartesianGroup& group, const FFTLayoutStruct& layout) = 0;
        /** \brief Create fully working plans, which must self-destruct in the PlanInterface's destructor. Use shared_ptr's. */
        virtual std::shared_ptr<PlanInterface<double>> getPlans_double(const MPICartesianGroup& group, const FFTLayoutStruct& layout) = 0;

        /** \brief Yes, another nested class interface: for the session guard. Your constructor should take care of your_library_init(), your destructor should take care of your_library_cleanup(). These are the session-wide initialization / finalization calls.
        */
        class SessionGuard {
        public:
            virtual ~SessionGuard() {};
        };
        virtual std::shared_ptr<SessionGuard> getSessionGuard(bool pVerbose) = 0;

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */



//    public:
#ifdef TEMPLATTEST
//        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/fftlibraryinterface_test.h"
#endif


#endif
