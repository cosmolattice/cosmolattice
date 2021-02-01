#ifndef TEMPLAT_PARALLEL_MPI_COMM_MPICARTESIANGROUP_H
#define TEMPLAT_PARALLEL_MPI_COMM_MPICARTESIANGROUP_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"

#include "TempLat/parallel/mpi/comm/mpicommreference.h"
#include "TempLat/parallel/mpi/comm/mpidomainsplit.h"

namespace TempLat {
    
    MakeException(MPICartesianGroupException);
    
    /** \brief A class keeps the books regarding the process layout
     * relative to the physics lattice.
     *
     * Unit test: make test-mpicartesiangroup
     **/
    
    class MPICartesianGroup {
    public:
        /* Put public methods here. These should change very little over time. */
        /** \brief Constructor: templated for the optional proposed decomposition, if you are unhappy
         *        with the built-in decomposition.
         *
         *  \param mBaseGroup the MPI Comm from which you start (typically just MPICommReference() == MPI_COMM_WORLD).
         *  \param nDimensions the dimensionality of the Cartesian setup.
         *  \param (optional)decomposition A vector with your domain decomposition, overriding the defaults.
         *  \param (optional)periodic A vector which indicates periodicity (1=true or 0=false) of each dimension. Default is all periodic.
         */
        MPICartesianGroup(
                          MPICommReference baseGroup,
                          ptrdiff_t nDimensions,
                          std::vector<int> decomposition,
                          std::vector<int> periodic = std::vector<int>()
                          ) :
        mBaseGroup(baseGroup),
        mCartesianGroup(mBaseGroup),
        mCartesianGroup_onlyDividedDimensions(mBaseGroup),
        mNDimensions(nDimensions),
        mDecomposition(decomposition),
        mPeriodic(periodic),
        mNumDividedDimensions(1)
        {
            //            if ( nDimensions < 2 && mBaseGroup.size() > 1u ) throw MPICartesianGroupException("Sorry, we only do 2 or more dimensions, not", nDimensions, "when mpi size is larger than one.");
            
            verifyInput();
            
            createGroups();
        }
        
        /** \brief convenience short-hand constructor with MPI_COMM_WORLD */
        MPICartesianGroup(ptrdiff_t nDimensions, std::vector<int> decomposition) :
        MPICartesianGroup(MPICommReference(), nDimensions, decomposition)
        {  }
        
        /** \brief Get the MPI_Comm already. */
        MPI_Comm getComm() const { return mCartesianGroup; }
        MPI_Comm getComm_onlyDividedDimensions() const { return mCartesianGroup_onlyDividedDimensions; }
        MPICommReference getBaseComm() const { return mBaseGroup; }

        int getRank() { return mCartesianGroup.getRank(); }
        
        ptrdiff_t getNDimensions() { return mNDimensions; }
        
        /** \brief Returns the position of the current process in the cartesian grid. Values are relative
         *         to the rank layout, nothing else. */
        const std::vector<int>& getPosition() const {
            return mSelfPosition;
        }
        
        /** \brief Returns number of dimensions in which a splitting is performed. */
        const ptrdiff_t& getNumberOfDividedDimensions() const {
            return mNumDividedDimensions;
        }
        
        /** \brief Only for testing purposes: get the decomposition of this group to verify whatever. */
        const std::vector<int>& getDecomposition() const {
            return mDecomposition;
        }
        
        const auto size() const {
            return mBaseGroup.size();
        }
        
        friend
        std::ostream& operator<< (std::ostream& ostream, MPICartesianGroup mGr) {
            ostream << "MPICartesianGroup:\n  decomposition: " << mGr.getDecomposition() << "\n  this rank: " << mGr.getRank() << "\n  this position: " << mGr.getPosition() << "\n";
            return ostream;
        }
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICommReference mBaseGroup; /* the parent group */
        MPICommReference mCartesianGroup; /* split up in the number of dimensions that were passed in the constructor */
        MPICommReference mCartesianGroup_onlyDividedDimensions; /* split up only in those dimensions whose splitting > 1. Necessary for pfft for example. */
        ptrdiff_t mNDimensions;
        std::vector<int> mDecomposition;
        std::vector<int> mPeriodic;
        ptrdiff_t mNumDividedDimensions;
        std::vector<int> mSelfPosition;
        
        std::vector<int> fetchPosition(int ofRank) {
            std::vector<int> result(mDecomposition.size(), 0);
#ifndef NOMPI
            //sayMPI << this << "Getting coords: " << ofRank << ", " << result << " nd: " <<mNDimensions<< "\n";
            if ( 0 != MPI_Cart_coords(mCartesianGroup, ofRank, (int) result.size() , result.data() ) ) {
                throw MPICartesianGroupException("Could not get result from MPI_Cart_coords.");
            }
#endif
            return result;
        }
        
        void createGroups() {
#ifndef NOMPI
            mCartesianGroup = createOneGroup(mBaseGroup, mNDimensions, true);
            
            /* next, keep only those leading dimensions whose splitting is larger than one, and keeping at least the first entry */
            mNumDividedDimensions = mDecomposition.size();
            while ( mNumDividedDimensions > 1 && mDecomposition[mNumDividedDimensions - 1] < 2 ) --mNumDividedDimensions;
            
            mCartesianGroup_onlyDividedDimensions = createOneGroup(mCartesianGroup, mNumDividedDimensions, false /* no re-ordering! Already ordered. */);
#endif
            
            mSelfPosition = fetchPosition(mCartesianGroup.rank());
        }
        
        MPICommReference createOneGroup(MPICommReference fromGroup, ptrdiff_t nDim, bool reorder) {
            MPI_Comm newComm;
            
            
#ifndef NOMPI
            //sayMPI << this << "Cart create: " << nDim << " " << mDecomposition << " " << mPeriodic << " " << reorder << "\n";
            MPI_Cart_create(mBaseGroup, nDim, mDecomposition.data(), mPeriodic.data(), reorder, &newComm);
#else
            newComm = MPI_COMM_WORLD;
#endif
            return MPICommReference(newComm);
        }
        
        void verifyInput() {
            while ( (ptrdiff_t) mDecomposition.size() < mNDimensions) mDecomposition.push_back(1);
            while ( mPeriodic.size() < mDecomposition.size()) mPeriodic.push_back(1);
            
            if ( (ptrdiff_t) mDecomposition.size() > mNDimensions ) mDecomposition.resize(mNDimensions);
            if ( (ptrdiff_t) mPeriodic.size() > mNDimensions ) mPeriodic.resize(mNDimensions);

            /* verify that decomposition doesn't violate the group size */
            ptrdiff_t groupSize = mBaseGroup.size();
            auto&& product = [&]() {
                ptrdiff_t result = 1;
                for (auto&& it : mDecomposition) result *= it;
                return result;
            };
            if ( product() < groupSize ) {
                say << "Decomposition " << mDecomposition << " yields fewer ranks than current group size " << groupSize << ". Either provide a smaller MPI_Comm, or accept that we override your decomposition. Overriding now.\n";
                mDecomposition[0] = groupSize;
            }
            for ( auto it = mDecomposition.rbegin(); it != mDecomposition.rend(); ++it) {
                if (product() > groupSize) {
                    *it = 1;
                }
            }
        }
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup_test.h"
#endif


#endif
