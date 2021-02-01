#ifndef TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_H
#define TEMPLAT_LATTICE_MEMORY_MEMORYLAYOUTS_TRANSPOSITIONMAP_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <vector>
#include <numeric>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"

namespace TempLat {

    MakeException(TranspositionMapOutOfBounds);

    /** \brief A class which maps between two orderings.
     * 
     * Unit test: make test-transpositionmap
     **/

    class TranspositionMap {
    public:
        /** \brief Default constructor: untransposed, both maps are a linear range. */
        TranspositionMap(ptrdiff_t nDimensions) :
        mFromAtoB(nDimensions),
        mFromBtoA(nDimensions)
        {
            std::iota(mFromAtoB.begin(), mFromAtoB.end(), 0);
            std::iota(mFromBtoA.begin(), mFromBtoA.end(), 0);
        }

        size_t size() const {
            return mFromAtoB.size();
        }

        ptrdiff_t getForward(ptrdiff_t index) const { return mFromAtoB[index]; }

        ptrdiff_t getInverse(ptrdiff_t index) const { return mFromBtoA[index]; }
        
        /** \brief Provide your forward mapping, which will be the new output of getForward. */
        template <typename T = ptrdiff_t>
        void setMap(std::vector<T> input) {
            mFromAtoB = std::vector<ptrdiff_t>(input.begin(), input.end());
            mFromBtoA.resize(mFromAtoB.size());
            for ( ptrdiff_t i = 0, iEnd = mFromAtoB.size(); i < iEnd; ++i) {
                if ( mFromAtoB[i] < 0 || mFromAtoB[i] > iEnd - 1) throw TranspositionMapOutOfBounds("Your map has entries that go beyond the size of the map:", input);
                mFromBtoA[mFromAtoB[i]] = i;
            }
        }
        
        bool isUntransposed() const {
            bool untransposed = true;
            for ( ptrdiff_t i = 0, iEnd = mFromAtoB.size(); i < iEnd; ++i) {
                untransposed = untransposed && mFromAtoB[i] == i && mFromBtoA[i] == i;
            }
            return untransposed;
        }
        
        bool isTransposed() const {
            return ! isUntransposed();
        }
        
        friend bool operator==(const TranspositionMap& a, const TranspositionMap& b) {
            bool equal = a.size() == b.size();
            for ( ptrdiff_t i = 0, iEnd = a.size(); i < iEnd && equal; ++i) {
                equal = equal && a.getForward(i) == b.getForward(i) &&  a.getInverse(i) == b.getInverse(i);
            }
            return equal;
        }
        
        friend std::ostream& operator<< ( std::ostream& ostream, const TranspositionMap& a) {
            ostream << " forward " << a.mFromAtoB << ", inverse " << a.mFromBtoA;
            return ostream;
        }
        
    private:
        /* From C to D means that entry at position 0 in C is at position mFromCtoD[0] in D. */
        std::vector<ptrdiff_t> mFromAtoB;
        std::vector<ptrdiff_t> mFromBtoA;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/transpositionmap_test.h"
#endif


#endif
