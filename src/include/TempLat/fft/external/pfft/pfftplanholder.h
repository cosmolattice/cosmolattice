#ifndef TEMPLAT_FFT_EXTERNAL_PFFT_PFFTPLANHOLDER_H
#define TEMPLAT_FFT_EXTERNAL_PFFT_PFFTPLANHOLDER_H
 
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
#include "TempLat/fft/fftlibraryinterface.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

namespace TempLat {

    /** \brief A class which implements part of FFTLibraryInterface::PlanInterface, holding the pfft plans in shared_ptr's - only freed when the last instance of this class is destructed.
     * 
     * Unit test: make test-pfftplanholder
     **/

    template < typename T>
    class PFFTPlanHolder : public FFTLibraryInterface::PlanInterface<T> {
    public:
        
#ifndef NOFFTFLOAT
        typedef typename std::conditional< std::is_same<float, T>::value, pfftf_plan, pfft_plan>::type plan;
#else
        typedef pfft_plan plan;
#endif
        /* Put public methods here. These should change very little over time. */
        PFFTPlanHolder(MPICartesianGroup group, plan planR2C, plan planC2R) :
        mGroup(group), /* need a reference to the group, to make sure the group is alive as long as the plans are alive. */
        mPlanR2C(std::make_shared<plan>(planR2C)),
        mPlanC2R(std::make_shared<plan>(planC2R))
        {
        }
        
        virtual ~PFFTPlanHolder() {
            if ( mPlanR2C.use_count() < 2) {
say << "PFFTPlanHolder is correctly being destructed.\n";
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
            pfft_destroy_plan(somePlan);
        }
        
#ifndef NOFFTFLOAT
        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        destroy(plan somePlan) {
            pfftf_destroy_plan(somePlan);
        }
#endif
        template <typename S = T>
        typename std::enable_if<std::is_same<S, double>::value, void>::type
        execute_r2c(plan somePlan, MemoryBlock<S> &mBlock) {
            pfft_execute_dft_r2c(somePlan, mBlock.ptr(), (pfft_complex*) mBlock.ptr());
        }
        
#ifndef NOFFTFLOAT
        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        execute_r2c(plan somePlan, MemoryBlock<S> &mBlock) {
            pfftf_execute_dft_r2c(somePlan, mBlock.ptr(), (pfftf_complex*) mBlock.ptr());
        }
#endif

        template <typename S = T>
        typename std::enable_if<std::is_same<S, double>::value, void>::type
        execute_c2r(plan somePlan, MemoryBlock<S> &mBlock) {
            //sayMPI << "PFFT double c2r starting.\n";
            pfft_execute_dft_c2r(somePlan, (pfft_complex*) mBlock.ptr(), mBlock.ptr());
            //sayMPI << "PFFT double c2r done.\n";
        }
        
#ifndef NOFFTFLOAT
        template <typename S = T>
        typename std::enable_if<std::is_same<S, float>::value, void>::type
        execute_c2r(plan somePlan, MemoryBlock<S> &mBlock) {
            pfftf_execute_dft_c2r(somePlan, (pfftf_complex*) mBlock.ptr(), mBlock.ptr());
        }
#endif

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/fft/external/pfft/pfftplanholder_test.h"
#endif


#endif
