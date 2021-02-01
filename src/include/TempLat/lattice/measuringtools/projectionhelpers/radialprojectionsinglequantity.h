#ifndef TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEQUANTITY_H
#define TEMPLAT_LATTICE_MEASUREMENTS_PROJECTIONHELPERS_RADIALPROJECTIONSINGLEQUANTITY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <limits>
#include <algorithm>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/exception.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsingledatum.h"
#include "TempLat/parallel/mpi/comm/mpicommreference.h"

namespace TempLat {
    
    MakeException(RadialProjectionSingleQuantityException);
    
    /** \brief A class which holds properties of a quantity (average, variance, min, max),
     *  each in a separate vector. Useful during the integration, will be transposed after
     *  the integration is done.
     *
     *
     * Unit test: make test-radialprojectionsinglequantity
     **/
    template <typename T>
    class RadialProjectionSingleQuantity {
        
    public:
        
        RadialProjectionSingleQuantity(ptrdiff_t size) :
        mAverages(size, 0),
        mVariances(size, 0),
        mMins(size, std::numeric_limits<T>::max()),
        mMaxs(size, -std::numeric_limits<T>::max())
        {}
        
        size_t size() const { return mAverages.size(); }

        /** \brief Add two workspaces together, element by element */
        void add(ptrdiff_t i, const RadialProjectionSingleQuantity& other) {
            checkBounds(i);
            mAverages[i] += other.mAverages[i];
            mVariances[i] += other.mVariances[i];
            mMins[i] = std::min(mMins[i], other.mMins[i]);
            mMaxs[i] = std::max(mMaxs[i], other.mMaxs[i]);
        }

        /** \brief Add one new weighted value to the collection of properties. */
        void add(ptrdiff_t i, const T& value, const T& weight) {
            checkBounds(i);
            mAverages[i] += weight * value;
            mVariances[i] += weight * value * value;
            mMins[i] = std::min(mMins[i], value);
            mMaxs[i] = std::max(mMaxs[i], value);
        }

        void clear() {
            mAverages.clear();
            mVariances.clear();
            mMins.clear();
            mMaxs.clear();
        }

        /** \brief This is why we keep stuff in vectors, sum up all the results from all processes in an easy way: vectors of the same things and same size just add up. */
        void finalize(MPICommReference comm) {
            /* reduce! */
            comm.Allreduce(&mAverages, MPI_SUM);
            comm.Allreduce(&mVariances, MPI_SUM);
            comm.Allreduce(&mMins, MPI_MIN);
            comm.Allreduce(&mMaxs, MPI_MAX);
        }

        /** \brief to be called only after finalize, returning the normalized result, this time transposed: all info per bin, instead of all bins per info. */
        RadialProjectionSingleDatum<T> getFinal(ptrdiff_t i, const T& multiplicity) {
            checkBounds(i);
            return RadialProjectionSingleDatum<T> (
                mAverages[i],
                mVariances[i],
                mMins[i],
                mMaxs[i],
                multiplicity
            );
        }
        
        template <typename S>
        friend class RadialProjectionResult;
        
    private:
        std::vector<T> mAverages;
        std::vector<T> mVariances;
        std::vector<T> mMins;
        std::vector<T> mMaxs;

        inline void checkBounds(ptrdiff_t i) {
#ifdef CHECKBOUNDS
            if ( i < 0 || i >= (ptrdiff_t) mAverages.size()) {
                throw RadialProjectionSingleQuantityException("Out of bounds: ", i, "not in", 0, " -- ", mAverages.size());
            }
#endif
        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsinglequantity_test.h"
#endif


#endif
