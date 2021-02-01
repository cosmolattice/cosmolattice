#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_DOEVAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/haseval.h"

namespace TempLat {


    /** \brief A class which helps implementing the intermediate evaluation mechanism, extremely useful for matrix algebra.
     *
     * 
     * Unit test: make test-doeval
     **/

    class DoEval {
    public:
        /* Put public methods here. These should change very little over time. */

        template <typename U>
        static inline
        typename std::enable_if<HasEval<U>::value, void>::type
        eval( U&& obj, ptrdiff_t i) {
            obj.eval(i);
        }

        template <typename U>
        static inline
        typename std::enable_if<!HasEval<U>::value, void>::type
        eval( U&& obj, ptrdiff_t i) {
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        DoEval() {

        }

    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/doeval_test.h"
#endif


#endif
