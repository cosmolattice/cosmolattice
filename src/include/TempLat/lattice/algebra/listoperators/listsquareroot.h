#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTSQUAREROOT_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_LISTSQUAREROOT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "listpower.h"

namespace TempLat {


    /** \brief A class which implements the square root over lists.
     *
     * 
     * 
     * Unit test: make test-listsquareroot
     **/

    class ListSquareRootTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename R>
    typename std::enable_if<(IsSTDGettable<0,R>::value || IsTempLatGettable<0,R>::value ), ListPower<R,HalfType>>::type
    sqrt(const R& r)
    {
        return ListPower<R,HalfType>(r, HalfType());
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/listsquareroot_test.h"
#endif


#endif
