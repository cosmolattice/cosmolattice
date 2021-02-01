#ifndef TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_FFTW_FFTWPLANHOLDER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <type_traits>

#ifndef NOFFT
#ifndef NOMPI
#include "fftw3-mpi.h"
#endif
#endif

#ifndef NOFFT
#include "fftw3.h"
#endif

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

namespace TempLat {

    /** \brief A class which holds FFT plans, and implements FFTLibraryInterface::PlanInterface<T>.
     * The plans are implemented in std::shared_ptr's, so they are only desctructed when the last instance is destructed.
     *
     * Unit test: make test-fftwplanholder
     **/

    template < typename T>
    class FFTWPlanHolder : public FFTLibraryInterface::PlanInterface<T> {
    public:
        
#ifndef NOFFTFLOAT
        typedef typename std::conditional< std::is_same<float, T>::value, fftwf_plan, fftw_plan>::type plan;
#else
        typedef fftw_plan plan;
#endif
        /* Put public methods here. These should change very little over time. */
        FFTWPlanHolder(MPICartesianGroup group, plan planR2C, plan planC2R) :
        mGroup(group), /* need a reference to the group, to make sure the group is alive as long as the plans are alive. */
        mPlanR2C(std::make_shared<plan>(planR2C)),
        mPlanC2R(std::make_shared<plan>(planC2R))
        {
        }
        
        ~FFTWPlanHolder() {
            if ( mPlanR2C.use_count() < 2) {
                //say << "FFTWPlanHolder is correctly being destructed.\n";
                destroy(*mPlanR2C);
                destroy(*mPlanC2R);
            }
        }
        
        
        
        virtual void c2r(MemoryBlock<T> &mBlock) { execute_c2r(*mPlanC2R, mBlock); };
        virtual void r2c(MemoryBlock<T> &mBlock) { execute_r2c(*mPlanR2C, mBlock); };
        
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICartesianGroup mGroup;
        std::shared_ptr<plan> mPlanR2C, mPlanC2R;
        
        template <typename S = T>
        typename std::enable_if<std::is_same<S, double>::value, void>::type
        destroy(plan somePlan) {
            fftw_destroy_plan(somePlan);
        }
        
#ifndef NOFFTFLOAT
        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        destroy(plan somePlan) {
            fftwf_destroy_plan(somePlan);
        }
#endif
        template <typename S = T>
        typename std::enable_if<std::is_same<S, double>::value, void>::type
        execute_r2c(plan somePlan, MemoryBlock<S> &mBlock) {
            //sayMPI << "FFTW double r2c starting. Plan: " << somePlan << "\n";
            fftw_execute_dft_r2c(somePlan, mBlock.ptr(), (fftw_complex*) mBlock.ptr());
            //sayMPI << "FFTW double r2c done.\n";
        }
        
#ifndef NOFFTFLOAT
        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        execute_r2c(plan somePlan, MemoryBlock<S> &mBlock) {
            fftwf_execute_dft_r2c(somePlan, mBlock.ptr(), (fftwf_complex*) mBlock.ptr());
        }
#endif

        template <typename S = T>
        typename std::enable_if<std::is_same<S, double>::value, void>::type
        execute_c2r(plan somePlan, MemoryBlock<S> &mBlock) {
            //sayMPI << "FFTW double c2r starting. Plan: " << somePlan << "\n";
            //sayMPI << "with block size: " << mBlock.size() << "\n";
            fftw_execute_dft_c2r(somePlan, (fftw_complex*) mBlock.ptr(), mBlock.ptr());
            //sayMPI << "FFTW double c2r done.\n";
        }
        
#ifndef NOFFTFLOAT
        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        execute_c2r(plan somePlan, MemoryBlock<S> &mBlock) {
            fftwf_execute_dft_c2r(somePlan, (fftwf_complex*) mBlock.ptr(), mBlock.ptr());
        }
#endif
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/fftw/fftwplanholder_test.h"
#endif


#endif
