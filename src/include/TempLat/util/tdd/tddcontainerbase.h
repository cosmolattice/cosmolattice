#ifndef TEMPLAT_UTIL_TDD_TDDCONTAINERBASE_H
#define TEMPLAT_UTIL_TDD_TDDCONTAINERBASE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tddmacros.h"

namespace TempLat {

    /** \brief A class which provides the interface for TDDContainers.
      *
      * Unit test: make test-tddcontainerbase
      */

    class TDDContainerBase {
    public:
    /* Put public methods here. These should change very little over time. */
        virtual int Test() = 0;

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/tdd/tddcontainerbase_test.h"
#endif


#endif
