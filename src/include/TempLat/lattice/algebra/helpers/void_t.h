#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_VOID_T_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_VOID_T_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief A C++17 std::void_t feature, implemented for lower versions.
     *  https://en.cppreference.com/w/cpp/types/void_t
     *
     * Unit test: make test-void_t
     **/

    template< class... >
    using void_t = void;
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/void_t_test.h"
#endif


#endif
