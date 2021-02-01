#ifndef TEMPLAT_LATTICE_ALGEBRA_COORDINATES_COORDINATEVECTOR_H
#define TEMPLAT_LATTICE_ALGEBRA_COORDINATES_COORDINATEVECTOR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/memory/memorylayoutstate.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
namespace TempLat {


    /** \brief A class which implements the concept of a coordinate.
     *
     *
     * Unit test: make test-coordinatevector
     **/

    class CoordinateVector {
    public:
        /* Put public methods here. These should change very little over time. */
        CoordinateVector() {

        }
        void doWeNeedGhosts(ptrdiff_t i)
        {

        }
        virtual void confirmSpace(ptrdiff_t i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) = 0;
        ptrdiff_t confirmGhostsUpToDate(ptrdiff_t i)
        {
          return 1;
        }
        inline virtual JumpsHolder getJumps() = 0;





    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/coordinates/coordinatevector_test.h"
#endif


#endif
