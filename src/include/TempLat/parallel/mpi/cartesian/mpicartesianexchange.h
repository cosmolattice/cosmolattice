#ifndef TEMPLAT_PARALLEL_MPI_CARTESIAN_MPICARTESIANEXCHANGE_H
#define TEMPLAT_PARALLEL_MPI_CARTESIAN_MPICARTESIANEXCHANGE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesianneighbours.h"
#include "TempLat/parallel/mpi/mpitags.h"

namespace TempLat {

    /** \brief A class which handles the exchange between neighbours in the cartesian group.
     *  Has two methods: exchangeUp and exchangeDown, which take as input a datatype, a dimension
     *  (because you need to specify what is up and down),
     *  a pointer for the sending memory and a pointer
     *  for the receiving memory.
     *
     * Unit test: make test-mpicartesianexchange
     **/

    class MPICartesianExchange {
    public:
        /* Put public methods here. These should change very little over time. */
        MPICartesianExchange(MPICartesianGroup group) :
        mGroup(group),
        mNeighbours(mGroup)
        {

        }

        void exchangeUp(MPI_Datatype dataType, ptrdiff_t dimension, void *ptrSend, void *ptrReceive) {
#ifndef NOMPI
            MPI_Status stat;
            static constexpr int sendCount = 1;
            MPI_Sendrecv(
                         ptrSend,
                         sendCount,
                         dataType,
                         mNeighbours.getUpperNeighbour(dimension),
                         MPITags::dataShiftGhostCells,
                         ptrReceive,
                         sendCount,
                         dataType,
                         mNeighbours.getLowerNeighbour(dimension),
                         MPITags::dataShiftGhostCells,
                         mGroup.getComm(),
                         &stat
                         );
#endif
        }

        void exchangeDown(MPI_Datatype dataType, ptrdiff_t dimension, void *ptrSend, void *ptrReceive) {
#ifndef NOMPI
            MPI_Status stat;
            static constexpr int sendCount = 1;
            MPI_Sendrecv(
                         ptrSend,
                         sendCount,
                         dataType,
                         mNeighbours.getLowerNeighbour(dimension),
                         MPITags::dataShiftGhostCells,
                         ptrReceive,
                         sendCount,
                         dataType,
                         mNeighbours.getUpperNeighbour(dimension),
                         MPITags::dataShiftGhostCells,
                         mGroup.getComm(),
                         &stat
                         );
#endif
        }

        void IsendUp(MPI_Datatype dataType, ptrdiff_t dimension, void *ptrSend, void *ptrReceive) {
#ifndef NOMPI
            //MPI_Status stat;
            static constexpr int sendCount = 1;
            MPI_Isend(
                      ptrSend,
                      sendCount,
                      dataType,
                      mNeighbours.getUpperNeighbour(dimension),
                      MPITags::dataShiftGhostCells,
                      mGroup.getComm(),
                      &mRequests[2]
                     );
#endif
        }
        void IrecvUp(MPI_Datatype dataType, ptrdiff_t dimension, void *ptrSend, void *ptrReceive) {
#ifndef NOMPI
            //MPI_Status stat;
            static constexpr int sendCount = 1;
            MPI_Irecv(
                      ptrReceive,
                      sendCount,
                      dataType,
                      mNeighbours.getLowerNeighbour(dimension),
                      MPITags::dataShiftGhostCells,
                      mGroup.getComm(),
                      &mRequests[0]
                     );
#endif
        }
        void IsendDown(MPI_Datatype dataType, ptrdiff_t dimension, void *ptrSend, void *ptrReceive) {
#ifndef NOMPI
            //MPI_Status stat;
            static constexpr int sendCount = 1;
            MPI_Isend(
                      ptrSend,
                      sendCount,
                      dataType,
                      mNeighbours.getLowerNeighbour(dimension),
                      MPITags::dataShiftGhostCells,
                      mGroup.getComm(),
                      &mRequests[3]
                      );
#endif
        }
        void IrecvDown(MPI_Datatype dataType, ptrdiff_t dimension, void *ptrSend, void *ptrReceive) {
#ifndef NOMPI
            //MPI_Status stat;
            static constexpr int sendCount = 1;
            MPI_Irecv(
                      ptrReceive,
                      sendCount,
                      dataType,
                      mNeighbours.getUpperNeighbour(dimension),
                      MPITags::dataShiftGhostCells,
                      mGroup.getComm(),
                      &mRequests[1]
                      );
#endif
        }
        void waitall()
        {
#ifndef NOMPI
          MPI_Status stat;
          MPI_Waitall(4,mRequests.data(),&stat);
#endif
        }



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICartesianGroup mGroup;
        MPICartesianNeighbours mNeighbours;
#ifndef NOMPI
        std::array<MPI_Request,4> mRequests;
#endif

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/cartesian/mpicartesianexchange_test.h"
#endif


#endif
