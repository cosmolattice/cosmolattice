#ifndef COSMOINTERFACE_SU2ALGEBRA_HELPERS_SU2DOUBLETGET_H
#define COSMOINTERFACE_SU2ALGEBRA_HELPERS_SU2DOUBLETGET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"


namespace TempLat {


    /** \brief A class which return the SU2DouletGet method.
     *
     * 
     * Unit test: make test-su2doubletget
     **/

    class SU2DoubletGetter {
    public:

        template<typename R, int N>
        static inline auto get(R&& r, Tag<N> t)
        {
            return r.SU2DoubletGet(t);
        }

    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doubletget_test.h"
#endif


#endif
