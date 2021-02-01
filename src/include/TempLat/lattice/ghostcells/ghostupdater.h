#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTUPDATER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <cstring>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/parallel/mpi/mpitypeconstants.h"
#include "TempLat/parallel/mpi/mpitags.h"
#include "TempLat/parallel/mpi/cartesian/mpicartesianexchange.h"
#include "TempLat/lattice/memory/jumpsholder.h"
#include "TempLat/lattice/ghostcells/ghostsubarraymap.h"

//#define IEXCH

namespace TempLat {

    MakeException(GhostUpdaterException);

    /** \brief A class which updates the ghost cells in our total memory block.
     * By having the JumpsHolder, this class knows what is the ghostDepth.
     *
     * Has one public method, update<T>(T* ptr), which, based on JumpsHolder,
     * uses the associated subarrays and performs the exchange up and down
     * in all dimensions, through calls to MPICartesianExchange with the
     * appriate datatypes for the subarrays.
     *
     *
     * Unit test: make test-ghostupdater
     **/

    class GhostUpdater {
    public:
        /* Put public methods here. These should change very little over time. */
        GhostUpdater(MPICartesianExchange exchange, JumpsHolder jumpsHolder) :
        mExchange(exchange),
        mJumpsHolder(jumpsHolder),
        mGhostDepth(mJumpsHolder.getPadding()[0][0]),
        mGhostSubarrayMap(mJumpsHolder, mGhostDepth),
        mNDimensions(mJumpsHolder.getSizesInMemory().size())
        {

            /* verify that */
            bool allSame = true;
            for ( auto&& it : mJumpsHolder.getPadding() ) {
                allSame = allSame && mGhostDepth == it[0] && mGhostDepth == it[1];
            }
            if ( ! allSame ) throw GhostUpdaterException("Can only work with identical padding at start and end of each dimension, not this.", allSame);
        }

        template <typename T>
        void update(T *ptr) {
#ifndef NOMPI
            pUpdate(ptr);
#else
            pUpdate_NOMPI(ptr);
#endif
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        MPICartesianExchange mExchange;
        JumpsHolder mJumpsHolder;
        ptrdiff_t mGhostDepth;
        GhostSubarrayMap mGhostSubarrayMap;
        ptrdiff_t mNDimensions;


        template <typename T>
        void pUpdate(T *ptr) {
            /* iterate dimensions */
            for ( ptrdiff_t d = 0 ; d < mNDimensions; ++d ) {
                update_forDimension(ptr, d);
            }
        }

        template <typename T>
        void update_forDimension(T *ptr, ptrdiff_t dimension) {
            /* get neighbours */
#ifndef NOMPI
#ifndef IEXCH
            mExchange.exchangeUp(
                                 mGhostSubarrayMap.getSubArray<T>(dimension),
                                 dimension,
                                 /* base ptr is lower corner of all memory, including ghosts. */
                                 /* send:
                                  Don't jump to origin, but jump along the edge of dimension
                                  to the point where we still have mGhostDepth until the end of
                                  our *owned* memory (before the mGhostDepth hyper slices start) */
                                 ptr + (mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                                 /* receive: in origin, including ghosts. */
                                 ptr
                                 );

            /* pointers: the same as above, but shifted by ghostDepth and ordering swapped. Yes. */
            mExchange.exchangeDown(
                                   mGhostSubarrayMap.getSubArray<T>(dimension),
                                   dimension,
                                   ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                                   ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension]
                                   );
#else
            mExchange.IrecvUp(
                              mGhostSubarrayMap.getSubArray<T>(dimension),
                              dimension,
                              /* base ptr is lower corner of all memory, including ghosts. */
                              /* send:
                               Don't jump to origin, but jump along the edge of dimension
                               to the point where we still have mGhostDepth until the end of
                               our *owned* memory (before the mGhostDepth hyper slices start) */
                              ptr + (mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                              /* receive: in origin, including ghosts. */
                              ptr
                              );
            /* pointers: the same as above, but shifted by ghostDepth and ordering swapped. Yes. */
            mExchange.IrecvDown(
                                mGhostSubarrayMap.getSubArray<T>(dimension),
                                dimension,
                                ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                                ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension]
                              );
            /*Same as above*/
            mExchange.IsendUp(
                              mGhostSubarrayMap.getSubArray<T>(dimension),
                              dimension,
                              ptr + (mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                              ptr
                            );
            mExchange.IsendDown(
                                mGhostSubarrayMap.getSubArray<T>(dimension),
                                dimension,
                                ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                                ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension]
                              );
            mExchange.waitall();
#endif
#endif
        }

        template <typename T>
        void pUpdate_NOMPI(T *ptr, ptrdiff_t dimension = 0) {
            /* for each dimension, walk all steps.
             When at the start of a ghost block in that dimension, copy that whole block,
             and jump past it.
             When in the 'owned memory' range of that dimension, recurse into deeper dimensions.
             */
            ptrdiff_t blockSize = mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension];


            /* front to back */
            std::memmove(ptr + (mGhostDepth + mJumpsHolder.getSizesInMemory()[dimension]) * mJumpsHolder.getJumpsInMemoryOrder()[dimension], ptr + mGhostDepth * mJumpsHolder.getJumpsInMemoryOrder()[dimension], blockSize * sizeof(T) );

            /* back to front */
            std::memmove(ptr, ptr + mJumpsHolder.getSizesInMemory()[dimension] * mJumpsHolder.getJumpsInMemoryOrder()[dimension], blockSize * sizeof(T) );


            if ( dimension < mNDimensions - 1 ) {
                /* already copied blocks which end up in this dimension's ghosting, but do not skip that:
                 * Just as in the MPI case, we need to copy all blocks including the ghosting,
                 * such that after a full cycle, also the corner blocks are properly copies into place.
                 */
                for ( ptrdiff_t i = - mGhostDepth; i < mJumpsHolder.getSizesInMemory()[dimension] + mGhostDepth; ++i ) {
                    /* so yes, i starts at mGhostDepth, *and* we add another mGhostDepth here:
                     we iterate our owned memory, which starts at ptr + mGhostDepth, and then
                     we skip mGhostDepth slices, because we already copied those entirely. */
                    pUpdate_NOMPI(
                                  ptr + (mGhostDepth + i) * mJumpsHolder.getJumpsInMemoryOrder()[dimension],
                                  dimension + 1
                                  );
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
#include "TempLat/lattice/ghostcells/ghostupdater_test.h"
#endif


#endif
