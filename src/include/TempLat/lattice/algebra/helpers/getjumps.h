#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETJUMPS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETJUMPS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasjumpmethods.h"
#include "TempLat/lattice/memory/jumpsholder.h"

namespace TempLat {

    /** \brief A class which gets jumps from all classes, also those that do not have jumps.
     *
     * Unit test: make test-getjumps
     **/



        class GetJumps {
        public:
        /* Put public methods here. These should change very little over time. */

            template <typename U>
            static inline
            typename std::enable_if<HasJumpMethods<U>::value, JumpsHolder>::type
            apply( U& obj) {
                return obj.getJumps();
            }

            template <typename U>
            static inline
            typename std::enable_if<!HasJumpMethods<U>::value, JumpsHolder>::type
            apply( U& obj) {
                return JumpsHolder();
            }

        private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
            GetJumps() {

            }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getjumps_test.h"
#endif


#endif
