#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCALTRANSPOSED_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocal.h"
#include "TempLat/lattice/memory/memorylayouts/transpositionmap.h"

namespace TempLat {

    MakeException(LayoutStructLocalTransposedSizeException);

    /** \brief
     *  transitionMap: a map of the dimensions, for transposition in memory. E.g. {0, 1, 2, 3} is untransposed 4d, {1, 0, 2, 3} is FFTW transposed.
     *                 In other words, the entries in transitionMap are in line with the entries in localStarts/-Sizes, and their value indicates the
     *                 physical-space dimension of that entry.
     * 
     * 
     * Unit test: make test-layoutstructlocaltransposed
     **/

    class LayoutStructLocalTransposed {
    public:
        /* Put public methods here. These should change very little over time. */
        LayoutStructLocalTransposed(std::vector<ptrdiff_t> initNGrid ) :
        mLocal(initNGrid),
        mTranspositionMap_memoryToGlobalSpace(initNGrid.size()),
        mSizesInMemory(mLocal.getLocalSizes())
        {

        }

        inline LayoutStructLocal& getLocal() { return mLocal; }
        inline const LayoutStructLocal& getLocal() const { return mLocal; }
        inline LayoutStructGlobal& getGlobal() { return getLocal().getGlobal(); }
        inline const LayoutStructGlobal& getGlobal() const { return getLocal().getGlobal(); }

        bool isTransposed() const {
            return mTranspositionMap_memoryToGlobalSpace.isTransposed();
        }

        template <typename T = ptrdiff_t>
        void setLocalSizes(const std::vector<T>& input) {
            getLocal().setLocalSizes(input);
            adaptMemorySizesFromTranspositionMap();
        }


        template <typename T = ptrdiff_t>
        void setTranspositionMap_memoryToGlobalSpace(const std::vector<T>& input) {
            checkSize("setTranspositionMap_memoryToGlobalSpace", input);
            mTranspositionMap_memoryToGlobalSpace.setMap(input);
            adaptMemorySizesFromTranspositionMap();
        }
        const TranspositionMap& getTranspositionMap_memoryToGlobalSpace() const { return mTranspositionMap_memoryToGlobalSpace; }

        const std::vector<ptrdiff_t>& getSizesInMemory() const { return mSizesInMemory; }

        /** \brief A dictionary for return values for memory to coordinate mapping. */
        struct CoordinateMapping {
            ptrdiff_t atIndex, withValue;
        };

        /** \brief With transposition, go from actual memory index in memoryDimension to spatial coordinate value at spatial dimension. */
        CoordinateMapping getSpatialLocationFromMemoryIndex(ptrdiff_t index, ptrdiff_t memoryDimension) const {
            CoordinateMapping result;
            result.atIndex = mTranspositionMap_memoryToGlobalSpace.getForward(memoryDimension);
            /* in this function we pass the new forward transposed index to the mLocal. */
            result.withValue = mLocal.memoryIndexToSpatialCoordinate(index, result.atIndex);
            return result;
        }

        /** \brief With transposition, go from spatial coordinate value at spatial dimension to actual memory index in memoryDimension. */
        CoordinateMapping getMemoryIndexFromSpatialLocation(ptrdiff_t position, ptrdiff_t spatialDimension) const {
            CoordinateMapping result;
            result.atIndex = mTranspositionMap_memoryToGlobalSpace.getInverse(spatialDimension);
            /* in this function we pass the input untransposed index to the mLocal. */
            result.withValue = mLocal.spatialCoordinateToMemoryIndex(position, spatialDimension);
            return result;
        }

        friend bool operator==(const LayoutStructLocalTransposed &a, const LayoutStructLocalTransposed &b) {
            
            bool result =
            a.mLocal == b.mLocal
            && a.mTranspositionMap_memoryToGlobalSpace == b.mTranspositionMap_memoryToGlobalSpace
            && a.mSizesInMemory.size() == b.mSizesInMemory.size();

            for ( size_t i = 0; i < a.mSizesInMemory.size(); ++i) {
                result = result && a.mSizesInMemory[i] == b.mSizesInMemory[i];
            }
            return result;
        }

        friend std::ostream& operator<<(std::ostream& ostream, const LayoutStructLocalTransposed& ls) {
            ostream << ls.mLocal << "\n"
            << "  SizesInMemory: " << ls.mSizesInMemory << "\n"
            << "  TranspositionMap_memoryToGlobalSpace: " << ls.mTranspositionMap_memoryToGlobalSpace << "\n";
            return ostream;
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        LayoutStructLocal mLocal;
        TranspositionMap mTranspositionMap_memoryToGlobalSpace;
        std::vector<ptrdiff_t> mSizesInMemory;

        void adaptMemorySizesFromTranspositionMap() {
            for ( ptrdiff_t i = 0, iEnd = std::min(mSizesInMemory.size(), mTranspositionMap_memoryToGlobalSpace.size()); i < iEnd; ++i) {
                ptrdiff_t getSizeIndex = mTranspositionMap_memoryToGlobalSpace.getForward(i);
                mSizesInMemory[i] = mLocal.getLocalSizes()[getSizeIndex];
            }
        }

        /** \brief Internal function for checking size of various inputs */
        template <typename T>
        void checkSize(const char* caller, const std::vector<T>& input) {
            if (input.size() != mTranspositionMap_memoryToGlobalSpace.size()) throw LayoutStructLocalTransposedSizeException("Input of", caller, "has wrong dimensionality:", input.size(), ", should have been", mTranspositionMap_memoryToGlobalSpace.size());
        }


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocaltransposed_test.h"
#endif


#endif
