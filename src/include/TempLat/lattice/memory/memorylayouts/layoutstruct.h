#ifndef TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H
#define TEMPLAT_FFT_MEMORYLAYOUTS_LAYOUTSTRUCT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <numeric>
#include <memory>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/memory/memorylayouts/hermitianpartners.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstructlocaltransposed.h"

namespace TempLat {
    
    MakeException(LayoutStructWrongSizeException);
    MakeException(LayoutStructOutOfBoundsException);
    
    /** \brief Holds the result of FFTLibraryInterface::computeLocalSizes.
     *  In itself, has practically no logic, only forwards all the getters and setters to their respective owners.
     *  See LayoutStructGlobal, LayoutStructLocal and LayoutStructLocalTransposed.
     *
     *
     * Unit test: make test-layoutstruct
     **/
    
    /** \brief The result holder for computeLocalSizes. It is your responsability to set all values. */
    struct LayoutStruct {
        LayoutStruct(std::vector<ptrdiff_t> initNGrid ) :
        mTransposed(initNGrid),
        mHermitianPartners(std::make_shared<HermitianPartners>(initNGrid))
        {

        }

        /** \brief An almost constructor: return a new instance which has a default global FFT layout */
        static LayoutStruct createGlobalFFTLayout( std::vector<ptrdiff_t> initNGrid) {
            LayoutStruct result(initNGrid);
            result.getGlobal().getGlobalSizes().back() = result.getGlobal().getGlobalSizes().back() / 2 + 1;
            result.getLocal().getLocalSizes().back() = result.getGlobalSizes().back();
            return result;
        }
        
        template <typename T = double>
        T getMaxRadius() const {
            return getGlobal().getMaxRadius<T>();
        }

        bool isTransposed() const {
            return getTransposed().isTransposed();
        }
        
        /** \brief local index in some dimension of the memory layout, goes into its corresponding spatial dimension
         *  in the target memory. No bounds checking!
         */
        void putSpatialLocationFromMemoryIndexInto(ptrdiff_t index, ptrdiff_t memoryDimension, std::vector<ptrdiff_t> &target) const {
            auto map = getTransposed().getSpatialLocationFromMemoryIndex(index, memoryDimension);
            target[map.atIndex] = map.withValue;
        }

        /** \brief Inverse of putSpatialLocationFromMemoryIndexInto: from spatial
         *  coordinate to memory indices, in memory-layout order (that is,
         *  transposed, ready to be applied to `JumpsHolder::getJumpsInMemoryOrder()`.
         */
        void putMemoryIndexFromSpatialLocationInto(ptrdiff_t position, ptrdiff_t spatialDimension, std::vector<ptrdiff_t> &target) const {
            auto map = getTransposed().getMemoryIndexFromSpatialLocation(position, spatialDimension);
            target[map.atIndex] = map.withValue;
        }

        const std::vector<ptrdiff_t>& getGlobalSizes() const { return getGlobal().getGlobalSizes(); }
        
        template <typename T = ptrdiff_t>
        void setLocalSizes(const std::vector<T>& input) {
            getTransposed().setLocalSizes(input);
        }
        std::vector<ptrdiff_t>& getLocalSizes() { return getLocal().getLocalSizes(); }
        const std::vector<ptrdiff_t>& getLocalSizes() const { return getLocal().getLocalSizes(); }

        const std::vector<ptrdiff_t>& getSizesInMemory() const { return getTransposed().getSizesInMemory(); }
        
        template <typename T = ptrdiff_t>
        void setLocalStarts(const std::vector<T>& input) {
            getLocal().setLocalStarts(input);
        }
        std::vector<ptrdiff_t>& getLocalStarts() { return getLocal().getLocalStarts(); }
        const std::vector<ptrdiff_t>& getLocalStarts() const { return getLocal().getLocalStarts(); }

        template <typename T = ptrdiff_t>
        void setTranspositionMap_memoryToGlobalSpace(const std::vector<T>& input) {
            getTransposed().setTranspositionMap_memoryToGlobalSpace(input);
        }
        const auto& getTranspositionMap_memoryToGlobalSpace() const { return getTransposed().getTranspositionMap_memoryToGlobalSpace(); }

        void setHermitianPartners(std::shared_ptr<HermitianPartners> newInstance) { mHermitianPartners = newInstance; }
        const auto& getHermitianPartners() const { return mHermitianPartners; }
        
        
        
        friend bool operator==(const LayoutStruct &a, const LayoutStruct &b) {
            
            bool result = a.mTransposed == b.mTransposed
            && *(a.mHermitianPartners) == *(b.mHermitianPartners);
 
            return result;
        }
        
        friend std::ostream& operator<<(std::ostream& ostream, const LayoutStruct& ls) {
            ostream << ls.mTransposed << "\n"
            << "  Hermitian layout: " << *(ls.mHermitianPartners) << "\n";
            return ostream;
        }
        
//        friend class FFTLayoutStruct;
//        friend class TripleStateLayouts;
        
    private:
        LayoutStructLocalTransposed mTransposed;
        /** \brief signed wavenumber and coordinate x = index > n/2 ? index - n : index. Need to provide this n/2 for each dimensions. */
        std::shared_ptr<HermitianPartners> mHermitianPartners;
        

        inline LayoutStructLocalTransposed& getTransposed() { return mTransposed; }
        inline LayoutStructLocal& getLocal() { return getTransposed().getLocal(); }
        inline LayoutStructGlobal& getGlobal() { return getLocal().getGlobal(); }

        inline const LayoutStructLocalTransposed& getTransposed() const { return mTransposed; }
        inline const LayoutStructLocal& getLocal() const { return getTransposed().getLocal(); }
        inline const LayoutStructGlobal& getGlobal() const { return getLocal().getGlobal(); }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
        
    };
    
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/memory/memorylayouts/layoutstruct_test.h"
#endif


#endif
