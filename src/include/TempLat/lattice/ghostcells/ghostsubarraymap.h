#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTSUBARRAYMAP_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTSUBARRAYMAP_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/ghostcells/ghostsubarray.h"

namespace TempLat {

    /** \brief A class which keeps track of all subarray types for a given
     *  layout, which in turn is described by the JumpsHolder. The map is
     *  for datatypes and dimensions. getSubArray<T>(dimension) is the
     *  only necessary public method.
     *
     * 
     * Unit test: make test-ghostsubarraymap
     **/

    class GhostSubarrayMap {
    public:
    /* Put public methods here. These should change very little over time. */
        GhostSubarrayMap(JumpsHolder jumpsHolder, ptrdiff_t ghostDepth) :
        mJumpsHolder(jumpsHolder),
        mGhostDepth(ghostDepth),
        mNDimensions(mJumpsHolder.getSizesInMemory().size())
        {

        }

        template <typename T>
        GhostSubarray getSubArray(ptrdiff_t dimension) {
            return pGetSubArray<T>(dimension);
        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        JumpsHolder mJumpsHolder;
        ptrdiff_t mGhostDepth;
        ptrdiff_t mNDimensions;
        std::map<MPI_Datatype, std::vector<GhostSubarray>> mSubArrays;


        template <typename T>
        GhostSubarray pGetSubArray(ptrdiff_t dimension) {
            MPI_Datatype dataType = MPITypeSelect<T>();
            if ( mSubArrays.count(dataType) < 1 ) {
                std::vector<GhostSubarray> arraySet;
                for ( ptrdiff_t idimension = 0; idimension < mNDimensions; ++idimension ) {
                    arraySet.emplace_back(
                                            mJumpsHolder,
                                            idimension,
                                            mGhostDepth,
                                            dataType
                                            );
                }
                mSubArrays[dataType] = arraySet;
            }
            return mSubArrays[dataType][dimension];
        }
        


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/ghostcells/ghostsubarraymap_test.h"
#endif


#endif
