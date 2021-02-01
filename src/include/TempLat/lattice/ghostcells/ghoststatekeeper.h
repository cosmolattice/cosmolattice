#ifndef TEMPLAT_LATTICE_MANIPULATION_GHOSTSTATEKEEPER_H
#define TEMPLAT_LATTICE_MANIPULATION_GHOSTSTATEKEEPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A class which only holds the state of ghost cells: stale or up to date.
     *
     * Unit test: make test-ghoststatekeeper
     **/

    class GhostStateKeeper {
    public:
    /* Put public methods here. These should change very little over time. */
        GhostStateKeeper() :
        mCurrentlyStale(true),
        mCurrentlyUpToDate(false)
        { }
        void setStale() {
            mCurrentlyUpToDate = false;
            mCurrentlyStale = true;
        }
        void setUpToDate() {
            mCurrentlyUpToDate = true;
            mCurrentlyStale = false;
        }
        const bool& isStale() const {
            return mCurrentlyStale;
        }
        const bool& isUpToDate() const {
            return mCurrentlyUpToDate;
        }
        const bool& isOutDated() const {
            return isStale();
        }
        const bool& needsUpdate() const {
            return isStale();
        }
        friend
        std::ostream& operator<< (std::ostream& ostream, const GhostStateKeeper& gsk) {
            ostream << "Ghost state: " << (gsk.isStale() ? "stale (needs update)" : (gsk.isUpToDate() ? "up to date" : "(BROKEN)"));
            return ostream;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        bool mCurrentlyStale;
        bool mCurrentlyUpToDate;


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/ghostcells/ghoststatekeeper_test.h"
#endif


#endif
