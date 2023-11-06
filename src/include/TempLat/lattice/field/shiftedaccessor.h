#ifndef TEMPLAT_LATTICE_FIELD_SHIFTEDACCESSOR_H_SHIFTEDACCESSOR_H
#define TEMPLAT_LATTICE_FIELD_SHIFTEDACCESSOR_H_SHIFTEDACCESSOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
//#include "TempLat/lattice/field/field.h"
#include "TempLat/lattice/algebra/operators/shiftedcoordinatesmanager.h"
#include "TempLat/lattice/loopers/looper.h"

namespace TempLat {


        /** \brief A class which shifts the field at a value displaced by one unit.
         *
         *
         * Unit test: make test-shiftedaccessor
         **/

        class ShiftedAccessor {
        public:
            /* Put public methods here. These should change very little over time. */
            ShiftedAccessor(const JumpsHolder& jump,std::vector<ptrdiff_t> shifts = std::vector<ptrdiff_t>()):
            mShifts(shifts)
            {
              this->updateJumps(jump);
            }



            const ptrdiff_t& getJump() const
            {
              return mShifts.memoryJump();
            }


//             ptrdiff_t shiftCoordinates(Looper& it)
//             {
//               ptrdiff_t originalOffset = it();
//
//               it()=(originalOffset + mShifts.memoryJump());
//               //for ( ptrdiff_t i = 0, iEnd = mShifts.size(); i < iEnd; ++i) it().mCoordinates[i] += mShifts.shift(i);
//               return originalOffset;
//             }
//
//             void unShiftCoordinates(Looper& it, const ptrdiff_t& originalOffset)
//             {
//               it()=(originalOffset);
//
//               //for ( ptrdiff_t i = 0, iEnd = mShifts.size(); i < iEnd; ++i) it().mCoordinates[i] -= mShifts.shift(i);
//             }



            void updateJumps(const JumpsHolder& jump)
            {
              mShifts.setJumps(jump);
            }

        private:
            /* Put all member variables and private methods here. These may change arbitrarily. */

            ShiftedCoordinatesManager mShifts;

        public:
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/shiftedaccessor_test.h"
#endif


#endif
