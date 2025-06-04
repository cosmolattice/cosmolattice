#ifndef TEMPLAT_PARALLEL_KOKKOS_SESSION_KOKKOSGUARD_H
#define TEMPLAT_PARALLEL_KOKKOS_SESSION_KOKKOSGUARD_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Franz R. Sattler,  Year: 2025


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/parallel/kokkos/kokkos.h"

namespace TempLat {
    MakeException(KokkosGuardInstantiationException);

    /**
     * @brief A class to manage and initialize the Kokkos runtime environment and its dependencies.
     *
     * The KokkosGuard class ensures proper initialization and finalization of the Kokkos runtime
     * environment and associated libraries such as MPI. It is designed to be instantiated exactly
     * once per process, typically at the beginning of the main function. The class handles thread
     * safety, MPI threading level verification, and manages a local sub-communicator for node-local
     * operations.
     *
     * Proper use of this class prevents multiple unwanted instances of Kokkos initialization and
     * ensures that all resources are cleaned up upon program termination.
     */
    class KokkosGuard {
    public:
        /* Put public methods here. These should change very little over time. */

        KokkosGuard(int argc, char **argv, bool verbose = false) : instanceProtectionKey(InstanceCounter(1)),
                                                                   mVerbose(verbose) {
            if (this->mVerbose) {
            }; /* just for the compiler warnings */

#ifndef NOKOKKOS
            Kokkos::initialize(argc, argv);
#endif
        }

        ~KokkosGuard() {
#ifndef NOKOKKOS
            Kokkos::finalize();
#endif
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        int instanceProtectionKey;
        bool mVerbose;

        /** \brief A sub-group of all the processes, which is local on the same node. This can be used to compute the best number of threads per process.
         By default we are greedy: use all the cpu power on a node. Set arguments upon launch to avoid that. (Not implemented yet.) */

        static inline int InstanceCounter(int delta = 0) {
            static int counter = 0;
            counter += delta;
            if (counter > 1) throw KokkosGuardInstantiationException(
                "Per process, the KokkosGuard can be instantiated only once. This should be done in `int main()`. This is wrong. Instances:",
                counter);
            return counter;
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/kokkos/session/kokkosguard_test.h"
#endif


#endif
