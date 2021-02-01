#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETDERIV_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETDERIV_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasderivmethod.h"
#include "TempLat/lattice/algebra/constants/zerotype.h"

namespace TempLat {

    /** \brief A class which gets derivates from all classes, also those that do not have derivative.
     *
     *
     * Unit test: make test-getderiv
     **/

    class GetDeriv {
    public:
    /* Put public methods here. These should change very little over time. */
        template <typename U, typename T>
        static inline
        typename std::enable_if<HasDerivMethod<U>::value, decltype(std::declval<U>().d(std::declval<T>()))>::type
        get( U& obj,  T& other) {
            return obj.d(other);
        }

        template <typename U, typename T>
        static inline
        typename std::enable_if<!HasDerivMethod<U>::value, ZeroType>::type
        get( U& obj,  T& other) {
            return ZeroType();
        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        GetDeriv() {

        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}/* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/getderiv_test.h"
#endif


#endif
