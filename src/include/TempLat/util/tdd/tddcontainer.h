#ifndef TEMPLAT_UTIL_TDD_TDDCONTAINER_H
#define TEMPLAT_UTIL_TDD_TDDCONTAINER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include "TempLat/util/tdd/tddcontainerbase.h"
#include "TempLat/util/tdd/tddassertion.h"
#include "TempLat/util/tdd/tddregister.h"
#include "TempLat/util/tdd/tddmacros.h"



namespace TempLat {

    /** \brief The container to instantiate if you want to register your class for testing.
      *
      * Unit test: make test-tddcontainer
      */
    template <class TESTME>
    class TDDContainer : public TDDContainerBase {
    public:
    /* Put public methods here. These should change very little over time. */
        TDDContainer() {
            TDDRegister::add(this, typeid(TESTME).name());
        }
        
        /** \brief Since we unsafely pass a pointer to `this` in our constructor, we must avoid copying this class at all times. */
        TDDContainer(const TDDContainer& other) = delete;

        /** \brief Since we unsafely pass a pointer to `this` in our constructor, we must avoid copying this class at all times. */
        TDDContainer& operator=(const TDDContainer& other) = delete;

        virtual int Test() {
            TDDAssertion assert;
            TESTME::Test(assert);
            sayShort << assert << "\n";
            return assert.FailCount();
        }

    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */


    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    class TDDContainerDummy {
    
    };

}

#ifdef TEMPLATTEST
#include "TempLat/util/tdd/tddcontainer_test.h"
#endif


#endif
