#ifndef TEMPLAT_SESSION_SESSIONGUARD_H
#define TEMPLAT_SESSION_SESSIONGUARD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/fft/fftlibraryselector.h"
#include "TempLat/parallel/mpi/session/mpiguard.h"

namespace TempLat {

    MakeException(SessionGuardInstantiationException);

    /** \brief A class which holds all the guards: fftw, pfft and mpi.
     * Only one instance per process is allowed. Throws an exception if
     * that condition is violated.
     *
     * Unit test: make test-sessionguard
     **/

    class SessionGuard {
    public:
    /* Put public methods here. These should change very little over time. */
        SessionGuard(int argc, char* argv[], bool verbose = true) :
        instanceProtectionKey(InstanceCounter(1)),
        mMPIGuard(argc, argv, verbose),
        mFFTSessionGuards(FFTLibrarySelector::getSessionGuards(verbose))
        {
        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        int instanceProtectionKey;
        MPIGuard mMPIGuard;
        std::vector<std::shared_ptr<FFTLibraryInterface::SessionGuard>> mFFTSessionGuards;

        static inline int InstanceCounter(int delta = 0) {
            static int counter = 0;
            counter += delta;
            if ( counter > 1) throw SessionGuardInstantiationException("Per process, the MPIGuard can be instantiated only once. This should be done in `int main()`. This is wrong. Instances:", counter);
            return counter;
        }


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/session/sessionguard_test.h"
#endif


#endif
