#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMSPACE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_CONFIRMSPACE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasspaceconfirmationmethods.h"
#include "TempLat/lattice/memory/memorylayouts/layoutstruct.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"

namespace TempLat {

    /** \brief A class which wraps the calls to confirm(Configuration/Fourier)Space, for classes that have it and objects that don't.
     *
     * Unit test: make test-confirmspace
     **/

    class ConfirmSpace {
    public:
    /* Put public methods here. These should change very little over time. */
        template <typename U>
        static inline
        typename std::enable_if<HasSpaceConfirmationMethods<U>::value, void>::type
        apply( U& obj, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
            obj.confirmSpace(newLayout, spaceType);
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasSpaceConfirmationMethods<U>::value, void>::type
        apply( U& obj, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
        }

        template <int N, typename U>
        static inline
        typename std::enable_if<HasSpaceConfirmationMethodsIndexed<N,U>::value, void>::type
        apply( U& obj, Tag<N> i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
            obj.confirmSpace(i, newLayout, spaceType);
        }

        template <int N, typename U>
        static inline
        typename std::enable_if<!HasSpaceConfirmationMethodsIndexed<N,U>::value, void>::type
        apply( U& obj, ptrdiff_t i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
        }

        template <typename U>
        static inline
        typename std::enable_if<HasSpaceConfirmationMethodsIndexedDyn<U>::value, void>::type
        apply( U& obj, ptrdiff_t i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
            obj.confirmSpace(i, newLayout, spaceType);
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasSpaceConfirmationMethodsIndexedDyn<U>::value, void>::type
        apply( U& obj, ptrdiff_t i, const LayoutStruct &newLayout, const SpaceStateInterface::SpaceType &spaceType) {
        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
        ConfirmSpace() {

        }



    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/confirmspace_test.h"
#endif


#endif
