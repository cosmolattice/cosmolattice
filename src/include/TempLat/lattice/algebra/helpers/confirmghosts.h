#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMGHOSTS_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMGHOSTS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasghostmethod.h"

namespace TempLat {

    /** \brief A class which wraps the calls to confirm(Configuration/Fourier)Space, for classes that have it and objects that don't.
     *
     * Unit test: make test-confirmspace
     **/


    class ConfirmGhosts {
    public:
    /* Put public methods here. These should change very little over time. */
        template <typename U>
        static inline
        typename std::enable_if<HasGhostMethod<U>::value, ptrdiff_t>::type
        apply( U& obj) {
            return obj.confirmGhostsUpToDate();
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasGhostMethod<U>::value, ptrdiff_t>::type
        apply( U& obj) {
            return 0;
        }
    /* Put public methods here. These should change very little over time. */
        template <int N, typename U>
        static inline
        typename std::enable_if<HasGhostMethodIndexed<N, U>::value, ptrdiff_t>::type
        apply( U& obj, Tag<N> i) {
            return obj.confirmGhostsUpToDate(i);
        }

        template <int N, typename U>
        static inline
        typename std::enable_if<!HasGhostMethodIndexed<N,U>::value, ptrdiff_t>::type
        apply( U& obj, ptrdiff_t i) {
            return 0;
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        ConfirmGhosts() {

        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/confirmghosts_test.h"
#endif


#endif
