#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTGLOBAL_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_LAYOUTSTRUCTGLOBAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {
    
    /** \brief A class which holds some memory informations.
     *
     * 
     * Unit test: make test-layoutstructglobal
     **/
    
    class LayoutStructGlobal {
    public:
        /* Put public methods here. These should change very little over time. */
        LayoutStructGlobal(std::vector<ptrdiff_t> initNGrid ) :
        mGlobalSizes(initNGrid)
        {
            for(auto x : mGlobalSizes) mSignConversionMidpoint.emplace_back( x / 2);
        }
        
        std::vector<ptrdiff_t>& getGlobalSizes() { return mGlobalSizes; }
        const std::vector<ptrdiff_t>& getGlobalSizes() const { return mGlobalSizes; }

        /** returns the largest possible distance from the origin. mSignConversionMidpoint holds the
         *  maximum value of each dimension. Check that in localIndexToGlobalCoordinate.
         */
        template <typename T = double>
        T getMaxRadius() const {
            using namespace std;
            T r2 = 0;
            for ( auto&& it : mSignConversionMidpoint ) r2 += it * it;
            return sqrt(r2);
        }
        
        /** \brief For both configuration and fourier space, the index values are not the same as coordinate
         *  values. Assuming periodic boundary conditions, we get that always c = i > half ? i - N : i;
         *  Don't mix up the arguments! Does not do transposition, so input pre-transposed dimension!
         */
        ptrdiff_t memoryIndexToSpatialCoordinate(ptrdiff_t index, ptrdiff_t dimension) const {
            
            const ptrdiff_t& tSize = mSignConversionMidpoint[dimension];
            
            //            say << "index " << index << " -> " << globalIndexPosition << " for start " << mLocalStarts[dimension] << "\n";
            return index > tSize ? index - mGlobalSizes[dimension] : index;
        }
        
        
        /** \brief Inverse of memoryIndexToSpatialCoordinate: get memory from position. */
        ptrdiff_t spatialCoordinateToMemoryIndex(ptrdiff_t position, ptrdiff_t dimension) const {
            return (position >= 0 ? position : position + mGlobalSizes[dimension]);
        }

        friend struct LayoutStruct;

        friend bool operator==(const LayoutStructGlobal &a, const LayoutStructGlobal &b) {
            
            bool result =
               a.mGlobalSizes.size() == b.mGlobalSizes.size()
            && a.mSignConversionMidpoint.size() == b.mSignConversionMidpoint.size();

            for ( size_t i = 0; i < a.mGlobalSizes.size(); ++i) {
                result = result && a.mGlobalSizes[i] == b.mGlobalSizes[i];
                result = result && a.mSignConversionMidpoint[i] == b.mSignConversionMidpoint[i];
            }
            return result;
        }

        friend std::ostream& operator<<(std::ostream& ostream, const LayoutStructGlobal& ls) {
            ostream
            << "  GlobalSizes: " << ls.mGlobalSizes << "\n"
            << "  SignConversionMidpoint: " << ls.mSignConversionMidpoint << "\n";
            return ostream;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::vector<ptrdiff_t> mGlobalSizes;
        std::vector<ptrdiff_t> mSignConversionMidpoint;
        
        
        
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/layoutstructglobal_test.h"
#endif


#endif
