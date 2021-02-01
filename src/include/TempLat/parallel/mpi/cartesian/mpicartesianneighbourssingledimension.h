#ifndef TEMPLAT_PARALLEL_MPI_CARTESIAN_MPICARTESIANNEIGHBOURSSINGLEDIMENSION_H
#define TEMPLAT_PARALLEL_MPI_CARTESIAN_MPICARTESIANNEIGHBOURSSINGLEDIMENSION_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"

namespace TempLat {
    
    /** \brief A class which fetches and stores the two neighbours in a single dimension.
     *
     * Unit test: make test-mpicartesianneighbourssingledimension
     **/
    
    class MPICartesianNeighboursSingleDimension {
    public:
        /* Put public methods here. These should change very little over time. */
        MPICartesianNeighboursSingleDimension(
                                              MPICartesianGroup cartesianGroup,
                                              ptrdiff_t dimension
                                              ) :
        mCartesianGroup(cartesianGroup),
        mDimension(dimension),
        mUpperNeighbourRank(0),
        mLowerNeighbourRank(0)
        {
#ifndef NOMPI
            const int displacement = 1;
            MPI_Cart_shift(mCartesianGroup.getComm(),
                           mDimension,
                           displacement,
                           &mLowerNeighbourRank,
                           &mUpperNeighbourRank
                           );
#endif
        }
        
        const int& getUpper() const {
            return mUpperNeighbourRank;
        }
        
        const int& getLower() const {
            return mLowerNeighbourRank;
        }
        
        friend std::ostream& operator<< (std::ostream& ostream, const MPICartesianNeighboursSingleDimension &nsd) {
            ostream << "Dimension: " << nsd.mDimension << " - lower neighbour: " << nsd.mLowerNeighbourRank << " - upper neighbour: " << nsd.mUpperNeighbourRank;
            return ostream;
        }
        
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICartesianGroup mCartesianGroup;
        ptrdiff_t mDimension;
        int mUpperNeighbourRank;
        int mLowerNeighbourRank;
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/cartesian/mpicartesianneighbourssingledimension_test.h"
#endif


#endif
