#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_HERMITIANPARTNERS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <vector>
#include <cmath>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianredundancy.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianvalueaccounting.h"

namespace TempLat {

    /** \brief An almost abstract class which your FFT library must implement, which maps the redundant entries in
     *  the complex representation of the FFT of your real values data, to their hermitian-conjugate partners.
     *
     * Unit test: make test-hermitianpartners
     **/

    class HermitianPartners {
    public:
        /* Put public methods here. These should change very little over time. */
        HermitianPartners(std::vector<ptrdiff_t> initNGrid) :
        mNDimensions(initNGrid.size()),
        mNGrid(initNGrid)
        {  }
        
        virtual ~HermitianPartners() {  }
        
        /** \brief For testing purposes: track which entries in the layout carry redundant information, and if so, what information.
         * The default implementation returns HermitianRedundancy::none, which you could (uselessly) use for configuration-space layouts.
         */
        virtual HermitianRedundancy qualify(const std::vector<ptrdiff_t>& globalCoordinate) {
            return HermitianRedundancy::none;
        }


        /** \brief If the entry at your input globalCoordinate has a partner which is its hermitian conjugate,
         *  then return the coordinates to that partner. Otherwise return the input. No bounds checking!
         *  The default implementation copies the input coordinates, which you could (uselessly) use for configuration-space layouts.
         */
        virtual HermitianRedundancy putHermitianPartner(const std::vector<ptrdiff_t>& globalCoordinate, std::vector<ptrdiff_t> &target) {
            target.resize(globalCoordinate.size());
            for ( ptrdiff_t i = 0, iEnd = globalCoordinate.size(); i < iEnd; ++i) {
                target[i] = globalCoordinate[i];
            }
            return HermitianRedundancy::positivePartner;
        }
        
            
        /** \brief Compute the number of unique / independent real and imaginary floating point values in a memory layout. */
        virtual HermitianValueAccounting getNumberOfIndependentValues() {
            ptrdiff_t vol = 1;
            for(ptrdiff_t x : mNGrid) vol *= x;
            return HermitianValueAccounting(vol, 0);
        }

        virtual std::string toString() const {
            return "Default HermitianRedundancy describer (configuration space -> no hermitian symmetry).";
        }

        friend
        std::ostream& operator<< (std::ostream& ostream, const HermitianPartners& hp) {
            ostream << (&hp)->toString();
            return ostream;
        }
        
        friend
        bool operator==(const HermitianPartners& a, const HermitianPartners& b) {
            return a.mNDimensions == b.mNDimensions && a.mNGrid == b.mNGrid;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
            ptrdiff_t mNDimensions;
            std::vector<ptrdiff_t> mNGrid;

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/hermitianpartners_test.h"
#endif


#endif
