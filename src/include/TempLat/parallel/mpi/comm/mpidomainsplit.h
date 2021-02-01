#ifndef TEMPLAT_PARALLEL_MPI_COMM_MPIDOMAINSPLIT_H
#define TEMPLAT_PARALLEL_MPI_COMM_MPIDOMAINSPLIT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/factorize.h"

namespace TempLat {
    
    MakeException(MPIDomainSplitException);


    /** \brief A class which factorizes an integer into N roughly
     * equal-size integers. That is, our best guess for the domain
     * decomposition.
     *
     * Unit test: make test-mpidomainsplit
     **/

    class MPIDomainSplit {
    public:
    /* Put public methods here. These should change very little over time. */
        /** \brief Constructor.
         *
         *  \param total the number of processors in total in the group.
         *   Don't pass a prime number.
         *  \param nDimensions the total number of the domain.
         *  \param nDimensionsToSplit <= nDimensions, the number of factors
         *   which should multiply to `total`.
         *
         */
        MPIDomainSplit(
                       ptrdiff_t total,
                       ptrdiff_t nDimensions,
                       ptrdiff_t nDimensionsToSplit
                       ) :
        mTotal(total),
        mNDimensions(nDimensions),
        mNDimensionsToSplit(nDimensionsToSplit),
        mFactors(total),
        mDomainSplitting(nDimensions, 1) /* set to nDimension entries, each with value 1. */
        {
            if ( nDimensionsToSplit > nDimensions ) throw MPIDomainSplitException("You provided a larger number of dimensions to split than the number of dimensions.", nDimensionsToSplit, " > ", nDimensions);
            
            /* reverse sort */
            mFactors.sort(true);

            if ( (ptrdiff_t) mFactors.size() <= mNDimensionsToSplit ) {
                std::copy(mFactors.begin(), mFactors.end(), mDomainSplitting.begin());
            } else {
                
                /* put the first entry (largest) in the first dimension.
                   Then, subsequently, put the next factor in the first smallest dimension.
                   That is, smallest, and if there are several smallest, take the first. */
                for ( auto&& it : mFactors ) {
                    /* find the next smallest */
                    ptrdiff_t smallest = 0;
                    while (
                           smallest < mNDimensionsToSplit - 1
                           && mDomainSplitting[smallest + 1] < mDomainSplitting[smallest]
                           ) ++smallest;
                    mDomainSplitting[smallest] *= it;
                }
            }
            
            verifySelf();

        }
        
        /* for copying etc. */
        auto begin() { return mDomainSplitting.begin(); }
        auto end() { return mDomainSplitting.end(); }
        
        operator std::vector<ptrdiff_t>() const { return mDomainSplitting; }
        operator std::vector<int>() const {
            return std::vector<int> (mDomainSplitting.begin(), mDomainSplitting.end());
        }
        
        ptrdiff_t computeSize() { return end() - begin(); }

        friend std::ostream& operator<< (std::ostream& ostream, MPIDomainSplit dom) {
            ostream << "Domain splitting( " << dom.mTotal << ", " << dom.mNDimensions << ", " << dom.mNDimensionsToSplit << " ): " << dom.mDomainSplitting ;
//            ostream << "\nfor factors: " << dom.factors;
            return ostream;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        const ptrdiff_t mTotal, mNDimensions, mNDimensionsToSplit;
        Factorize mFactors;
        std::vector<ptrdiff_t> mDomainSplitting;

        void verifySelf() {
            /* just a check that everything worked out ok */
            ptrdiff_t checkTotal = 1;
            ptrdiff_t checkNSplit = 0;
            for ( auto&& it : mDomainSplitting ) {
                checkTotal *= it;
                if ( it != 1 ) ++checkNSplit;
            }
            if ( checkTotal != mTotal ) throw MPIDomainSplitException("Domain decomposition failed: ended up with ", checkTotal, "entries, instead of the imposed", mTotal);
            if ( checkNSplit > mNDimensionsToSplit ) {
                throw MPIDomainSplitException("Domain decomposition failed: split more dimensions than allowed.", checkNSplit, " > ", mNDimensionsToSplit);
            }
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/parallel/mpi/comm/mpidomainsplit_test.h"
#endif


#endif
