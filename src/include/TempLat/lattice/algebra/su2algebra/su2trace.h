#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2TRACE_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2TRACE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat {


    /** \brief A function which takes the trace of su2 matrices.
     *
     * 
     * Unit test: make test-su2trace
     **/


    template <typename R>
    auto trace(R&& r)
    {
        return 2 * r.SU2Get(0_c);
    }



} /* TempLat */

#include "TempLat/lattice/algebra/su2algebra/su2trace_test.h"


#endif
