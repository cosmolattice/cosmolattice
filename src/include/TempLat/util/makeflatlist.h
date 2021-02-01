#ifndef TEMPLAT_UTIL_MAKEFLATLIST_H
#define TEMPLAT_UTIL_MAKEFLATLIST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/flattentuple.h"
#include "TempLat/util/tuplemaker.h"

namespace TempLat {


    /** \brief A class which creates a flat tuple from potentially a list of composite objects.
     * 
     * 
     * Unit test: make test-makeflatlist
     **/

    template <typename... Args>
    auto make_flat_list(Args... args)
    {
        return flatten_tuple(std::make_tuple(make_tuple_from(args)...));
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/makeflatlist_test.h"
#endif


#endif
