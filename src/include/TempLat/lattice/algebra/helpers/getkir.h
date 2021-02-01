#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETKIR_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETKIR_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/lattice/algebra/helpers/haskir.h"
namespace TempLat {


    /** \brief A getter for kIR.
     *
     *
     * Unit test: make test-getkir
     **/

    class GetKIR { //Careful, this method works only on types which alsohave a get method. Need this to deduce the return type.
    public:
    /* Put public methods here. These should change very little over time. */

        template <typename U>
        static inline
        typename std::enable_if<HasKIR<U>::value, decltype(std::declval<U>().getKIR())>::type
        getKIR( U& obj) {
            return obj.getKIR();
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasKIR<U>::value, int>::type
        getKIR( U& obj) {
            return 1;
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
#include "TempLat/lattice/algebra/helpers/getkir_test.h"
#endif


#endif
