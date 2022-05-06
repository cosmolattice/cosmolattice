#ifndef TEMPLAT_PARALLEL_MPI_SESSION_MPIGUARD_H
#define TEMPLAT_PARALLEL_MPI_SESSION_MPIGUARD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/parallel/threads/threadsettings.h"
#include "TempLat/parallel/mpi/mpitypeconstants.h"

namespace TempLat {
    
    MakeException(MPIGuardInstantiationException);
    
    /** \brief A class which guards intialization and finalization of pure MPI.
     * Only used by MPIGuard, which manually calls its destructor, but which also holds the FFTW and PFFT guards.
     *
     * No public methods. What?!?? Yes. We made MPIGuard a friend,
     * so it has access to private methods, and hence is the only thing
     * in the world which can construct and hold an instance of FFTWGuard.
     *
     * Unit test: make test-mpiguardactualmpi
     **/
    
    class MPIGuard {
    public:
        /* Put public methods here. These should change very little over time. */
        
        MPIGuard(int argc, char** argv, bool verbose = false) :
        instanceProtectionKey(InstanceCounter(1)),
        mVerbose (verbose)
#ifndef NOMPI
        , nodeLocalGroup(MPI_COMM_NULL)
#endif
        {
            if ( this->mVerbose) { }; /* just for the compiler warnings */



#ifndef NOMPI
            int rank = 0;

            if (mVerbose ) sayShort << "Calling MPI_Init.\n";
            
            /* are threads allowed now? */
            int threads_ok = 1;
            int provided = 0;
            if (MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided) != MPI_SUCCESS ) throw std::runtime_error("MPI_Init_thread failed");
            threads_ok = provided >= MPI_THREAD_FUNNELED;
            /* pass the result on */
            if ( ! threads_ok ) ThreadSettings::setMPIThreadsNotOK();
            
            /* verify that there are no linker problems: */
            
            int inited = 1;
            MPI_Initialized(&inited);
            if ( ! inited) throw std::runtime_error("MPI_Initialized(int*) returned false after call to MPI_Init_threads. There probably is a problem with your linking to dynamic libraries.");
            
            if (mVerbose ) sayShort << "MPI Initialized.\n";
            
            /* what is the number of threads we can anyway use on this hardware? */
            MPI_Info empty = (MPI_Info) NULL;
            MPI_Info_create(&empty);
            MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, empty, &nodeLocalGroup);
            MPI_Info_free(&empty);
            if ( nodeLocalGroup != MPI_COMM_NULL ) {
                int localProcessNum = 1;
                MPI_Comm_size(nodeLocalGroup, &localProcessNum);
                /* pass the result on */
                ThreadSettings::setMPILocalSize(localProcessNum);
            }
            
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            
            int weAreTheMainThread = 0;
            MPI_Is_thread_main(&weAreTheMainThread);
            if ( ! weAreTheMainThread ) throw std::runtime_error("TempLatParallelization object is constructed by another thread than the main thread. This is illegal. You should never copy this object. Only construct a single one in your int main(), and leave it there.");
#endif

//            if ( rank == 0 ) std::cerr << ThreadSettings::getInstance() << "\n";
        }
        
        
        ~MPIGuard() {
#ifndef NOMPI
            int trueIfFinalized;
            MPI_Finalized(&trueIfFinalized);
            if (! trueIfFinalized ) {
                /* a barrier so that an unemployed process does not interrupt running processes. Not sure this is necessary, but it does no harm. */
                MPI_Barrier(MPI_COMM_WORLD);
                
                if (mVerbose ) sayShort << "Calling MPI_Finalize.\n";
                MPI_Finalize();
            }
#endif
        }
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        int instanceProtectionKey;
        bool mVerbose;

        /** \brief A sub-group of all the processes, which is local on the same node. This can be used to compute the best number of threads per process.
         By default we are greedy: use all the cpu power on a node. Set arguments upon launch to avoid that. (Not implemented yet.) */
#ifndef NOMPI
        MPI_Comm nodeLocalGroup;

#endif

        static inline int InstanceCounter(int delta = 0) {
            static int counter = 0;
            counter += delta;
            if ( counter > 1) throw MPIGuardInstantiationException("Per process, the MPIGuard can be instantiated only once. This should be done in `int main()`. This is wrong. Instances:", counter);
            return counter;
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/session/mpiguard_test.h"
#endif


#endif
