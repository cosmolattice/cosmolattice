#ifndef TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPISENDRECEIVE_H
#define TEMPLAT_PARALLEL_MPI_COMM_EXCHANGE_MPISENDRECEIVE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/parallel/mpi/comm/mpicommreference.h"

namespace TempLat {
    
    MakeException(MPISendReceiveException);
    
    
    /** \brief A class which holds all methods that do plain Send and Receive in MPI.
     * Just call the send and receive methods with any scalar or vector-like input, associated rank numbers and tags.
     * Unused for now.
     *
     * Unit test: make test-mpisendreceive
     **/
    
    class MPISendReceive {
    public:
        /* Put public methods here. These should change very little over time. */
        MPISendReceive(MPICommReference comm) :
        mpiComm(comm)
        {
            
        }
        
        template <typename T>
        void send(std::vector<T> *theVec, int toRank, int tag) {
            /* Vectors have a dynamic size. So we must perform two
             * information exchanges: the size and the data.
             */

            size_t theSize = theVec->size();

            send(&theSize, toRank, tag, 1);

            send(theVec->data(), toRank, tag + 1, theSize);

        }
        
        template <typename T>
        void receive(std::vector<T> *theVec, int fromRank, int tag) {
            /* Vectors have a dynamic size. So we must perform two
             * information exchanges: the size and the data.
             */

            size_t theSize = 0;

            receive(&theSize, fromRank, tag, 1);

            theVec->resize(theSize);

            receive(theVec->data(), fromRank, tag + 1, theSize);
        }
        
        template <typename T, size_t N>
        void send(std::array<T, N> *theArr, int toRank, int tag) {
            send(theArr->data(), toRank, tag, N) ;
        }
        
        template <typename T, size_t N>
        void receive(std::array<T, N> *theArr, int fromRank, int tag) {
            receive(theArr->data(), fromRank, tag, N);
        }
        
        /* these should work with built in types */
        template <typename T>
        void send(T *value, int toRank, int tag, int size = 1) {
#ifndef NOMPI
            int result = 0;
            result = MPI_Send(value, size, MPITypeSelect<T>(), toRank, tag, mpiComm);
            if ( 0 != result ) {
                throw MPISendReceiveException("MPI_Send failed. Error code:", result);
            }
#endif
        }
        
        template <typename T>
        void receive(T *value, int fromRank, int tag, int size = 1) {
#ifndef NOMPI
            int result = 0;
            MPI_Status status;
            status.MPI_ERROR = 0;
            result = MPI_Recv(value, size, MPITypeSelect<T>(), fromRank, tag, mpiComm, &status);
            if ( 0 != result || 0 != status.MPI_ERROR ) {
                throw MPISendReceiveException("MPI_Send failed. Error codes:", result, status.MPI_ERROR);
            }
#endif
        }
        
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICommReference mpiComm;
        
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/comm/exchange/mpisendreceive_test.h"
#endif


#endif
