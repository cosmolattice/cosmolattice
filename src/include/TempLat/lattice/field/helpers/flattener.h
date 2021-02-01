#ifndef TEMPLAT_LATTICE_FIELD_HELPERS_FLATTENER_H
#define TEMPLAT_LATTICE_FIELD_HELPERS_FLATTENER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/helpers/hasastuplecat.h"

namespace TempLat {


    /** \brief A class which flattents a composite collection.
     *
     * 
     * Unit test: make test-flattener
     **/

    class Flattener {
    public:
        /* Put public methods here. These should change very little over time. */

        template <typename U>
        static inline
        typename std::enable_if<HasAsTupleCat<U>::value, decltype(std::declval<U>().asTupleCat())>::type
        asTupleCat( U& obj) {
            return obj.asTupleCat();
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasAsTupleCat<U>::value, U>::type&
        asTupleCat( U& obj) {
            return obj;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };





} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/field/helpers/flattener_test.h"
#endif


#endif
