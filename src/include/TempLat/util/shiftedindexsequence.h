#ifndef TEMPLAT_UTIL_SHIFTEDINDEXSEQUENCE_H
#define TEMPLAT_UTIL_SHIFTEDINDEXSEQUENCE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {


    /** \brief
     * Use as shifted_index_sequence<SHIFT>(std::make_index_sequence<N>{})
     * To get an index sequence<SHIFT,SHIFT+1,...,SHIFT+N-1>
     *
     * Unit test: make test-shiftedindexsequence
     **/
     template<int SHIFT, size_t... I>
     auto shifted_index_sequence(std::index_sequence<I...>)
     {
       return std::index_sequence<(I+SHIFT)...>{};
     }

     class ShiftedIndexSequenceTester{
     public:
#ifdef TEMPLATTEST
       static inline void Test(TDDAssertion& tdd);
#endif

     };

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/util/shiftedindexsequence_test.h"
#endif


#endif
