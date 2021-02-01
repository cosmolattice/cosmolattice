#ifndef TEMPLAT_PARALLEL_MPI_CARTESIAN_MPICARTESIANNEIGHBOURS_H
#define TEMPLAT_PARALLEL_MPI_CARTESIAN_MPICARTESIANNEIGHBOURS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesiangroup.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesianneighbourssingledimension.h"

namespace TempLat {
    
    /** \brief A class which fetches and stores the neighbours in all dimensions.
     * Internally just contructs a std::vector<MPICartesianNeighboursSingleDimension>
     * with each entry corresponding to its associated dimension. 
     *
     * Unit test: make test-mpicartesianneighbours
     **/
    
    class MPICartesianNeighbours {
    public:
        /* Put public methods here. These should change very little over time. */
        MPICartesianNeighbours(
                               MPICartesianGroup group
                               ) :
        mCartesianGroup(group),
        mNDimensions(mCartesianGroup.getNDimensions())
        {
        
            for ( ptrdiff_t dim = 0; dim < mNDimensions; ++dim) {
                allNeighbours.emplace_back(mCartesianGroup, dim);
//                sayMPI << allNeighbours.back() << "\n";
            }
        
        }
        
        const int& getUpperNeighbour(ptrdiff_t dimension) {
//            sayMPI << "returning neighbour for dimension " << dimension << " upper " << allNeighbours[dimension].getUpper() << "\n";
            return allNeighbours[dimension].getUpper();
        }

        const int& getLowerNeighbour(ptrdiff_t dimension) {
//            sayMPI << "returning neighbour for dimension " << dimension << " lower " << allNeighbours[dimension].getLower() << "\n";
            return allNeighbours[dimension].getLower();
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICartesianGroup mCartesianGroup;
        ptrdiff_t mNDimensions;
        std::vector<MPICartesianNeighboursSingleDimension> allNeighbours;
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/cartesian/mpicartesianneighbours_test.h"
#endif


#endif
