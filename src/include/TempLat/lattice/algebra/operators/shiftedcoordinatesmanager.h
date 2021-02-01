#ifndef TEMPLAT_LATTICE_ALGEBRA_SHIFTEDCOORDINATESMANAGER_H
#define TEMPLAT_LATTICE_ALGEBRA_SHIFTEDCOORDINATESMANAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/jumpsholder.h"



namespace TempLat {
    MakeException(GetterShiftedGotWithoutSpaceConfirmationException);


    /** \brief A class which compute jumps for shifts.
     *
     *
     * Unit test: make test-shiftedcoordinatesmanager
     **/

    class ShiftedCoordinatesManager {
    public:
        /* Put public methods here. These should change very little over time. */
      ShiftedCoordinatesManager(std::vector<ptrdiff_t> shifts = std::vector<ptrdiff_t>()):
        mShifts(shifts),
        mMemoryJump(0),
        mNeverComputedJump(true)
        {

        }

      const ptrdiff_t& getBareOffset() const {
            if ( mNeverComputedJump ) throw GetterShiftedGotWithoutSpaceConfirmationException("Call to confirm(Config/Fourier)Space somehow missed us. Broken stuff.");
            return mMemoryJump;
        }
      inline
      void setJumps(const JumpsHolder& jump) {
          mJumps=jump;
          recomputeJump();
      }

      const ptrdiff_t& memoryJump() const
      {
        return mMemoryJump;
      }
      const ptrdiff_t& shift(ptrdiff_t i) const
      {
        return mShifts[i];
      }
      size_t size() const
      {
        return mShifts.size();
      }
      const std::vector<ptrdiff_t>& getShifts() const
      {
        return mShifts;
      }
      const bool& neverComputedJump() const
      {
        return mNeverComputedJump;
      }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        JumpsHolder mJumps;
        const std::vector<ptrdiff_t> mShifts;
        ptrdiff_t mMemoryJump;
        bool mNeverComputedJump;

        void recomputeJump() {
            mMemoryJump = 0;
            if ( ! mJumps.isEmpty() && mShifts.size() > 0u) {
                mMemoryJump = mJumps.getJump(mShifts);
                //say << "getterShifted has computed new jump: " << mMemoryJump << " from mShifts: " << mShifts << ", which at first position gives " << mJumps.toOrigin() << " + " << mMemoryJump << " = " << mJumps.toOrigin() + mMemoryJump << "\n";
            } else if (mShifts.size() > 0u) {
                say << "GetterShifted is computing total memory jump in confirm(Config/Fourier)Space, but got empty jumps.\n";
            }
           mNeverComputedJump = false;
       }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/operators/shiftedcoordinatesmanager_test.h"
#endif


#endif
