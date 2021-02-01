#ifndef TEMPLAT_PARALLEL_MPI_MPITAGS_H
#define TEMPLAT_PARALLEL_MPI_MPITAGS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


/* we are going to abuse __LINE__ as an looper like value, simply so we get unique values for all tags. */

    namespace MPITags {
        static constexpr int waitInLine = __LINE__;
        static constexpr int remapPrepareTransose = __LINE__;
        static constexpr int remapAllToAll = __LINE__;
        static constexpr int gatherAsVector = __LINE__;
        static constexpr int rangeExchangeSend = __LINE__;
        static constexpr int dataShiftGhostCells = __LINE__;
    };

    /** \brief A class which just compiles.
     * Unit test: make test-mpitags
     **/

    class MPITagsJustCompile {
    public:
    /* Put public methods here. These should change very little over time. */
        MPITagsJustCompile() {

        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/mpitags_test.h"
#endif


#endif
