#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_GETEVAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/hasgeteval.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"

namespace TempLat {


    /** \brief A class which chooses between get and getEval, to allow for some intermediate caching (useful for operations that derive from matrix
     * multiplication for example).
     *
     * 
     * Unit test: make test-geteval
     **/

    class GetEval {
    public:
        /* Put public methods here. These should change very little over time. */


        template <typename U>
        static inline
        typename std::enable_if<HasGetEval<U>::value, decltype(std::declval<U>().getEval(std::declval<ptrdiff_t>()))>::type
        getEval( U&& obj, ptrdiff_t i) {
            return obj.getEval(i);
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasGetEval<U>::value,  decltype(GetValue::get(std::declval<U>(),std::declval<ptrdiff_t>()))>::type
        getEval( U&& obj, ptrdiff_t i) {
            return GetValue::get(obj,i);
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        GetEval() {

        }
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/geteval_test.h"
#endif


#endif
