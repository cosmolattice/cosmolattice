#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTLOCAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructglobal.h"

namespace TempLat {

    MakeException(LayoutStructLocalSizeException);

    /** \brief A class which
     *  localSizes: the local size in each dimension.
     *  localStart: the coordinate of the first entry in local memory, in the global coordinate space.
     * 
     * 
     * Unit test: make test-layoutstructlocal
     **/

    class LayoutStructLocal {
    public:
        LayoutStructLocal(std::vector<ptrdiff_t> initNGrid ) :
        mGlobal(initNGrid),
        mLocalSizes( initNGrid),
        mLocalStarts(initNGrid.size(), 0)
        {

        }

        inline LayoutStructGlobal& getGlobal() { return mGlobal; }
        inline const LayoutStructGlobal& getGlobal() const { return mGlobal; }

        template <typename T = ptrdiff_t>
        void setLocalSizes(const std::vector<T>& input) {
            checkSize("setLocalSizes", input);
            mLocalSizes = std::vector<ptrdiff_t>(input.begin(), input.end());
        }
        std::vector<ptrdiff_t>& getLocalSizes() { return mLocalSizes; }
        const std::vector<ptrdiff_t>& getLocalSizes() const { return mLocalSizes; }

        template <typename T = ptrdiff_t>
        void setLocalStarts(const std::vector<T>& input) {
            checkSize("setLocalStarts", input);
            mLocalStarts = std::vector<ptrdiff_t>(input.begin(), input.end());
        }
        std::vector<ptrdiff_t>& getLocalStarts() { return mLocalStarts; }
        const std::vector<ptrdiff_t>& getLocalStarts() const { return mLocalStarts; }

        /** \brief For both configuration and fourier space, the index values are not the same as coordinate
         *  values. Assuming periodic boundary conditions, we get that always c = i > half ? i - N : i;
         *  Don't mix up the arguments! Does not do transposition, so input pre-transposed dimension!
         */
        ptrdiff_t memoryIndexToSpatialCoordinate(ptrdiff_t index, ptrdiff_t dimension) const {
            return mGlobal.memoryIndexToSpatialCoordinate(index + mLocalStarts[dimension], dimension);
        }
        
        
        /** \brief Inverse of memoryIndexToSpatialCoordinate: get memory from position. */
        ptrdiff_t spatialCoordinateToMemoryIndex(ptrdiff_t position, ptrdiff_t dimension) const {
            return mGlobal.spatialCoordinateToMemoryIndex(position, dimension) - mLocalStarts[dimension];
        }


        friend bool operator==(const LayoutStructLocal &a, const LayoutStructLocal &b) {
            
            bool result =
            a.mGlobal == b.mGlobal
            && a.mLocalSizes.size() == b.mLocalSizes.size()
            && a.mLocalStarts.size() == b.mLocalStarts.size();

            for ( size_t i = 0; i < a.mLocalSizes.size(); ++i) {
                result = result && a.mLocalSizes[i] == b.mLocalSizes[i];
                result = result && a.mLocalStarts[i] == b.mLocalStarts[i];
            }
            return result;
        }

        friend std::ostream& operator<<(std::ostream& ostream, const LayoutStructLocal& ls) {
            ostream << ls.mGlobal << "\n"
            << "  LocalSizes: " << ls.mLocalSizes << "\n"
            << "  LocalStarts: " << ls.mLocalStarts << "\n";
            return ostream;
        }

    private:
        LayoutStructGlobal mGlobal;
        std::vector<ptrdiff_t> mLocalSizes;
        std::vector<ptrdiff_t> mLocalStarts;


        /** \brief Internal function for checking size of various inputs */
        template <typename T>
        void checkSize(const char* caller, const std::vector<T>& input) {
            if (input.size() != mLocalSizes.size()) throw LayoutStructLocalSizeException("Input of", caller, "has wrong dimensionality:", input.size(), ", should have been", mLocalSizes.size());
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocal_test.h"
#endif


#endif
