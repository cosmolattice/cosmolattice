#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2GET_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2GET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"


namespace TempLat {


    /** \brief A class which return the SU2Get method.
     *
     * 
     * Unit test: make test-su2get
     **/

    class SU2Getter {
    public:

        template<typename R, int N>
        static inline auto get(R&& r, Tag<N> t)
        {
            return r.SU2Get(t);
        }

    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2get_test.h"
#endif


#endif
