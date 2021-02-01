#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNGRID_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETNGRID_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hastoolbox.h"


namespace TempLat {


    /** \brief A class which returns the number of grid points in a specific dimensions.
     *
     * 
     * Unit test: make test-getngrid
     **/

    class GetNGrid {
    public:
        /* Put public methods here. These should change very little over time. */
        template <typename U>
        static inline
        typename std::enable_if<HasToolBox<U>::value, ptrdiff_t>::type
        get( U&& obj) { // for isotropic lattices
            return obj.getToolBox()->mNGridPointsVec[0]; //Isotropic lattices only.
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasToolBox<U>::value, ptrdiff_t>::type
        get( U&& obj) {
            return 1;
        }

        template <typename U>
        static inline
        typename std::enable_if<HasToolBox<U>::value, std::vector<ptrdiff_t>>::type
        getVec( U& obj) { // for isotropic lattices
            return obj.getToolBox()->mNGridPointsVec; //Isotropic lattices only.
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasToolBox<U>::value, std::vector<ptrdiff_t>>::type
        getVec( U& obj) {
            return {};
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
        static inline void Test(TDDAssertion& tdd);
    };



} /* TempLat */

#include "TempLat/lattice/algebra/helpers/getngrid_test.h"


#endif
