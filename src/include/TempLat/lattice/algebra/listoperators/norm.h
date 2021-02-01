#ifndef TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_NORM_H
#define TEMPLAT_LATTICE_ALGEBRA_OPERATORS_LISTOPERATORS_NORM_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"

namespace TempLat {


    /** \brief A class which computes the norm of a list.
     *
     * 
     * Unit test: make test-norm
     **/

    template <typename R>
    typename std::enable_if<IsTempLatGettable<0,R>::value || IsSTDGettable<0,R>::value , decltype(total(pow<2>(std::declval<R>())))>::type
    norm2(const R& r)
    {
        return total(pow<2>(r));
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/listoperators/norm_test.h"
#endif


#endif
