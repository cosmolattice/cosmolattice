#ifndef TEMPLAT_LATTICE_MEMORY_JUMPSHOLDER_H
#define TEMPLAT_LATTICE_MEMORY_JUMPSHOLDER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <cmath>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/parallel/mpi/comm/mpicommreference.h"

namespace TempLat {

    MakeException(JumpsHolderException);
    MakeException(DifferentJumpsHolderException);

    /** \brief A class which holds jumps for one particular layout of the memory.
     *
     * Unit test: make test-jumpsholder
     **/

    class JumpsHolder {
    public:
    /* Put public methods here. These should change very little over time. */

        /** \brief Default constructor is only there to have it explicitly... */
        JumpsHolder() : mLayout({1}), mIsEmpty(true) {}

        /** \brief Constructor needs to know the following:
         *  \param mSizesInMemory The memory that should be iterated over.
         *  \param padding The padding on each end of each dimension. Padding is not in principle iterated over, but matters hugely for the jumps, of course.
         *  \param atomSize The last dimension by default has size one (e.g. one double, one float). But you may set it to for example 2, for complex pairs.
         */
        JumpsHolder( LayoutStruct layout, std::vector<std::array<ptrdiff_t, 2u>> padding, ptrdiff_t atomSize = 1) :
        mLayout(layout),
        mSizesInMemory(mLayout.getSizesInMemory()),
        mPadding(padding),
        mIsEmpty(false)
        {
            if ( mSizesInMemory.size() != padding.size() ) throw JumpsHolderException("Different sizes for layout and padding:", mSizesInMemory.size(), "!=", padding.size());

            mJumps.resize(mSizesInMemory.size());

            ptrdiff_t iStart = ((ptrdiff_t) mJumps.size()) - 1;

            ptrdiff_t jumpAccumulator = 1;

            for ( ptrdiff_t i = iStart; i > -1; --i) {

                ptrdiff_t nextDimSize = atomSize;

                if ( i < iStart) nextDimSize = mSizesInMemory[i + 1] + padding[i + 1][0] + padding[i + 1][1];

                jumpAccumulator *= nextDimSize;

                mJumps[i] = jumpAccumulator;

            }


            mJumpToIteratableStart = 0;

            for (ptrdiff_t i = 0, iEnd = mSizesInMemory.size(); i < iEnd; ++i) {

                mJumpToIteratableStart += padding[i][0] * mJumps[i];

            }

        }

        size_t size() const {
            return mJumps.size();
        }


        const ptrdiff_t& toOrigin() const {
            return mJumpToIteratableStart;
        }

        inline const std::vector<ptrdiff_t>& getSizesInMemory() const {
            return mSizesInMemory;
        }

        inline const std::vector<std::array<ptrdiff_t, 2u>>& getPadding() const {
            return mPadding;
        }

        inline const std::vector<ptrdiff_t>& getJumpsInMemoryOrder() const {
            return mJumps;
        }

        /** \brief Compute the total jump for a multidimensional offset/ shift vector: these take account for possible transposition. Input is expected to be in the space of the logical problem, NOT necessarily the ordering of memory. */
        inline ptrdiff_t getJump(const std::vector<ptrdiff_t>& shifts) const {
            if ( shifts.size() != mJumps.size() ) throw JumpsHolderException("Called ptrdiff_t getJump(std::vector<ptrdiff_t> shifts) with shift of different dimensionality than our jumps. Jumps size:", mJumps.size(), ", your input shifts size:", shifts.size());
            ptrdiff_t result = 0;
            for ( ptrdiff_t i = 0, iEnd = mJumps.size(); i < iEnd; ++i) {
                result += mJumps[mLayout.getTranspositionMap_memoryToGlobalSpace().getInverse(i)] * shifts[i];
            }
            return result;
        }

        /** \brief Compute the jump to a logical space coordinate (which can be negative),
         *  not from a memory index vector. Bounds checking is done: returns a negative
         *  offset if the requested coordinates are not in our space. No exception thrown.
         *  "position" is the global coordinate in the lattice, not the mpi-local one.
         */
        inline ptrdiff_t getTotalOffsetFromSpatialCoordinates(const std::vector<ptrdiff_t>& position) const {
            if ( position.size() != mJumps.size() ) throw JumpsHolderException("Called ptrdiff_t getTotalOffsetFromSpatialCoordinates(std::vector<ptrdiff_t> position) with position of different dimensionality than our jumps. Jumps size:", mJumps.size(), ", your input position size:", position.size());

            std::vector<ptrdiff_t> memoryIndices(position.size());

            for ( ptrdiff_t i = 0, iEnd = position.size(); i < iEnd; ++i) {
                mLayout.putMemoryIndexFromSpatialLocationInto(position[i], i, memoryIndices);
            }

            bool inBounds = true;
            for ( ptrdiff_t i = 0, iEnd = position.size(); i < iEnd; ++i) {
                inBounds = inBounds && memoryIndices[i] >= 0 && memoryIndices[i] < mSizesInMemory[i];
            }

            ptrdiff_t result = toOrigin();
            for ( ptrdiff_t i = 0, iEnd = mJumps.size(); i < iEnd; ++i) {
                result += mJumps[i] * memoryIndices[i];
            }
            //if ( ! inBounds ) sayMPI << "Detected out of bounds: " << position << " -> mem " << memoryIndices << " " << mLayout << "\n";
            return inBounds ? result: -1;
        }


/* REMOVED: ambiguous for the caller. Use the explicit 'getJump' interface. */
//        inline const ptrdiff_t& operator[](const ptrdiff_t &i) const {
//            return mJumps[i];
//        }

        inline bool isEmpty() const { return mIsEmpty; }

        friend std::ostream& operator<< ( std::ostream& ostream, const JumpsHolder& jh) {
            ostream << "  Jumps holder:\n    toOrigin: " << jh.toOrigin() << "\n    jumps: " << jh.getJumpsInMemoryOrder() << "\n";
            return ostream;
        }

        inline bool operator!=(const JumpsHolder& b) {
            return ! ( *this == b);
        }

        inline bool operator==(const JumpsHolder& b) {
            bool result = isEmpty() == b.isEmpty();
            result = result && mSizesInMemory.size() == b.mSizesInMemory.size();
            result = result && mPadding.size() == b.mPadding.size();
            result = result && mJumps.size() == b.mJumps.size();
            result = result && mJumpToIteratableStart == b.mJumpToIteratableStart;

            /* could compare every entry, but they should be equal as long as layout is equal, given that we already compared jumpToStart. */
            for ( ptrdiff_t i = 0, iEnd = mSizesInMemory.size(); i < iEnd && result; ++i) {
                result = result && mSizesInMemory[i] == b.mSizesInMemory[i];
            }

            return result;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        /** \brief The layout that we are jumping on, for compact referencing... */
        LayoutStruct mLayout;
        std::vector<ptrdiff_t> mSizesInMemory;
        std::vector<std::array<ptrdiff_t, 2u>> mPadding;
        /** \brief If there is padding, the first owned iterable memory is at some distance from the start of the memory block */
        ptrdiff_t mJumpToIteratableStart;
        /** \brief The final jumps in each direction. Add this to your (typed!) pointer, and you end up one step up or down in that dimension. */
        std::vector<ptrdiff_t> mJumps;

        bool mIsEmpty;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/jumpsholder_test.h"
#endif


#endif
