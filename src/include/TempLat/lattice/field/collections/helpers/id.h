#ifndef COSMOINTERFACE_COLLECTIONS_HELPERS_ID_H
#define COSMOINTERFACE_COLLECTIONS_HELPERS_ID_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief A class which returns the own type of its template parameter.
     *
     * 
     * Unit test: make test-id
     **/

    template <class T>
    struct id {
        using type = T;
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "id_test.h"
#endif


#endif
