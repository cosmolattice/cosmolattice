#ifndef TEMPLAT_PARALLEL_MPI_COMM_MPICOMMREFERENCE_H
#define TEMPLAT_PARALLEL_MPI_COMM_MPICOMMREFERENCE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <map>

#include "TempLat/util/exception.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/mpitypeconstants.h"
#include "TempLat/parallel/mpi/comm/exchange/mpiallreduce.h"

namespace TempLat {
    
    MakeException(MPICommReferenceBookKeepingException);
    
    /** \brief A class which holds an MPIComm. The Comm WILL DESTRUCT when
     * the last owner of a reference to this class destructs its instance.
     * Unless it is MPI_COMM_WORLD, of course.
     *
     *
     * Unit test: make test-mpicommreference
     **/
    
    class MPICommReference : public MPIAllReduce {
    public:
        /* Put public methods here. These should change very little over time. */
        MPICommReference(MPI_Comm comm = MPI_COMM_WORLD) :
        MPIAllReduce(comm),
        mComm(comm),
        mSize(getSizeForComm(comm)),
        mRank(getRankInComm(comm))
        {
            BookKeeper(mComm, true, false);
        }
        
        /** \brief Copy constructor: must implement this for the bookkeeping. */
        MPICommReference(const MPICommReference & other) :
        MPIAllReduce(other.mComm),
        mComm(other.mComm),
        mSize(other.mSize),
        mRank(other.mRank)
        {
            BookKeeper(mComm, true, false);
        }

        
        /** \brief Copy assignment: must implement this for the bookkeeping. */
        MPICommReference& operator=(const MPICommReference& other) {
            mComm = other.mComm;
            mSize = other.mSize;
            mRank = other.mRank;
            BookKeeper(mComm, true, false);
            return *this;
        }

        ~MPICommReference() {
#ifndef NOMPI
            ptrdiff_t referenceCount = BookKeeper(mComm, false, true);
            if ( mComm != MPI_COMM_WORLD && mComm != MPI_COMM_NULL && referenceCount < 1 ) {
                #ifdef TEMPLATTEST
                    sayMPI << "Freeing mpi comm: " << mComm << " with reference count " << referenceCount << "\n";
                #endif
                MPI_Comm_free(&mComm) ;
            }
#endif
        }
        
        operator MPI_Comm() const { return mComm; }
        
        int size() const {
            return mSize;
        }
        
        int rank() const {
            return mRank;
        }
        int getRank() const { return rank(); }
        static inline int worldRank() {
            /* only for debug and testing purposes */
            static bool set = false;
            static int rank = -1;
            if ( ! set ) {
                set = true;
                rank = MPICommReference().rank();
            }
            return rank;
        }
        
    private:
        /** Implementation considerations:
         * We could just have used a std::shared_ptr<MPI_Comm> here.
         * But we do not do that, because we can never be 100%
         * certain that the user always directly constructs a single
         * MPICommReference and copies that around, or accidently constructs
         * two instances of MPICommReference with the same MPI_Comm.
         * So, instead of std::shared_ptr<MPI_Comm> mpiCommPtr,
         * we use an internal std::map<MPI_Comm, ptrdiff_t> which
         * simply counts the number of construct / destruct calls
         * for each MPI_Comm.
         */
        MPI_Comm mComm;
        int mSize;
        int mRank;
        
        inline static ptrdiff_t BookKeeper(MPI_Comm comm, bool construct, bool destruct) {
            
            static std::map<MPI_Comm, ptrdiff_t> theBook;

            if ( comm == MPI_COMM_WORLD ) return 1; /* never free the world. */

            if ( construct != !destruct ) throw MPICommReferenceBookKeepingException("You called with contruct: ", construct, "destruct:", destruct);
            
            ptrdiff_t result = -1;
            
            if ( construct ) {
                
                if ( theBook.count(comm) == 0 ) {
                    theBook[comm] = 0;
                }
                result = ++theBook[comm];

            } else if ( destruct ) {
                
                if ( theBook.count(comm) < 1 ) {
                    throw MPICommReferenceBookKeepingException("Calling destruct for MPI_Comm which is not present in the Books.", comm, theBook.count(comm));
                } else if (theBook[comm] < 1 ) {
                    throw MPICommReferenceBookKeepingException("Calling destruct for MPI_Comm which already has a reference count < 1.", comm, "reference count:", theBook[comm]);
                }
                result = --theBook[comm];
                if ( result == 0 ) {
                    theBook.erase(comm);
                }
            }
            
            return result;
        }
        
        
        inline static int getSizeForComm(MPI_Comm comm) {
            int result = 1;
#ifndef NOMPI
            if ( comm != MPI_COMM_NULL ) {
                MPI_Comm_size(comm, &result);
            }
#endif
            return result;
        }

        inline static int getRankInComm(MPI_Comm comm) {
            int result = 0;
#ifndef NOMPI
            if ( comm != MPI_COMM_NULL ) MPI_Comm_rank(comm, &result);
#endif
            return result;
        }

        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/comm/mpicommreference_test.h"
#endif


#endif
