#ifndef TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_H
#define TEMPLAT_LATTICE_MEMORY_GHOSTBUSTER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <functional>
#include <cstring>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/jumpsholder.h"

namespace TempLat {
    
    MakeException(GhostBusterOrderException);
    MakeException(GhostBusterBoundsException);

    /** \brief A class which adds and removes (busts!) ghost cells to your layout.
     *  Pass the origin layout and the target layout to the constructor.
     *  Then pass any pointer which satisfies for origin layout to the function call operator. And the rest will be history.
     *
     *  ```
     *  GhostBuster egon(JumpsHolder from, JumpsHolder to);
     *  egon(ptr);
     *  ```
     *  Since the ghost cells are ghost cells, they will be overwritten arbitrarily. The non-ghost
     *  cells are guaranteed to port their value from the old to the new layout.
     *
     *  Memory allocation of the pointer is NOT affected. This is a memmove-like routine.
     *
     * Unit test: make test-ghostbuster
     **/
    
    class GhostBuster {
    public:
        /* Put public methods here. These should change very little over time. */
        GhostBuster(const JumpsHolder& from, const JumpsHolder& to, bool verbose = false) :
        mFrom(from),
        mTo(to),
        mDirection (mFrom.toOrigin() + mFrom.getJumpsInMemoryOrder()[0] - mTo.toOrigin() - mTo.getJumpsInMemoryOrder()[0] ),
        mNDimensions(mFrom.getJumpsInMemoryOrder().size())
        {
            /* actually, in place ghostbusting only works if the motion
             * is in the same direction for all dimensions.
             */
            bool consistent = true;
            for ( ptrdiff_t i = 0, iEnd = mFrom.getJumpsInMemoryOrder().size(); i < iEnd; ++i) {
                
                ptrdiff_t thisSign = mFrom.getJumpsInMemoryOrder()[i] - mTo.getJumpsInMemoryOrder()[i];
                if ( mDirection == 0 ) mDirection = thisSign;
                consistent = consistent && (thisSign * mDirection >= 0);
                
            }
            if(verbose) say << "from " << from << " to " << to << "\n";
            if ( ! consistent ) throw GhostBusterOrderException("Can only work with jumps that change in the same direction for all dimensions, not with this:", mFrom.getJumpsInMemoryOrder(), " to ", mTo.getJumpsInMemoryOrder());
        }
        
        /** \brief Do the transformation. Size is passed on, only used when compiled CHECKBOUNDS defined. */
        template <typename T>
        void operator()(T *ptr, ptrdiff_t memSize) {
            bustTheGhosts(ptr, memSize);
        }

        /** \brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
        template <template <typename S, typename... MArgs> class M, typename T, typename... Args>
        void operator()(M<T, Args...> &obj) {
            operator()((T*) obj.data(), obj.size());
        }
       
#ifdef __INTEL_COMPILER //for some reasons, icc does not underrstand the default nested template for zero arguments. 
/** \brief overload for passing objects which have a data() and a size() method, like std::vector<T> */
        template <template <typename S> class M, typename T>
        void operator()(M<T> &obj) {
            operator()((T*) obj.data(), obj.size());
        }
#endif

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        JumpsHolder mFrom, mTo;
        ptrdiff_t mDirection, mNDimensions;
        
        template <typename T>
        void bustTheGhosts(T *ptr, ptrdiff_t memSize) {
            recursor<T>(
                        mFrom.getSizesInMemory().data(),
                        ptr + mFrom.toOrigin(),
                        mFrom.getJumpsInMemoryOrder().data(),
                        ptr + mTo.toOrigin(),
                        mTo.getJumpsInMemoryOrder().data(),
                        0,
                        mNDimensions,
                        ptr + memSize
                        );
            
        }
        
        template <typename T>
        void recursor(const ptrdiff_t *layout,
                      T *fromPtr,
                      const ptrdiff_t *fromJumps,
                      T *toPtr,
                      const ptrdiff_t *toJumps,
                      ptrdiff_t thisDim,
                      ptrdiff_t nDim,
                      T *endPtr
                      ) {
            if ( thisDim < nDim - 1) {
                
                ptrdiff_t iStart = mDirection < 0 ? (*layout) - 1 : 0;
                ptrdiff_t iEnd = mDirection < 0 ? -1 : *layout;
                ptrdiff_t di = mDirection < 0 ? -1 : 1;
                
                for ( ptrdiff_t i = iStart; mDirection * i < mDirection * iEnd; i += di) {
//                    say << "recursor dim " << thisDim << " i " << i << " jump: " << i * (*fromJumps) << "\n";
                    recursor<T>(
                                layout + 1,
                                fromPtr + i * (*fromJumps),
                                fromJumps + 1,
                                toPtr + i * (*toJumps),
                                toJumps + 1,
                                thisDim + 1,
                                nDim,
                                endPtr
                                );
                }
            } else { /* move the rods */
                /* who're you gonna call? */
//                say << "forward - Moving " << *layout << " to " << toPtr << " end point " << (toPtr + *layout * sizeof(T))<< " from " << fromPtr << " end point " << (fromPtr + *layout * sizeof(T)) << " Correct? " << (mDirection * (ptrdiff_t)toPtr <= mDirection * (ptrdiff_t)fromPtr ? "YES" : "NO" ) << "\n";
#ifdef CHECKBOUNDS
                if (mDirection * (ptrdiff_t)toPtr > mDirection * (ptrdiff_t)fromPtr) throw GhostBusterOrderException("recursor: Overwriting valid memory in GhostBuster - the orders are wrong.", toPtr, mDirection < 0 ? " < " : " > ", fromPtr);
                
                if ( toPtr + *layout > endPtr || fromPtr + *layout > endPtr ) {
                    throw GhostBusterBoundsException("Detected memory access out of bounds. End pointer: ", endPtr, ", but write at end", toPtr + *layout, "and read at end", fromPtr + *layout);
                }
#endif
                std::memmove(toPtr, fromPtr, *layout * sizeof(T) );
            }
        }
        
        
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/ghostcells/ghostbuster_test.h"
#endif


#endif
