#ifndef TEMPLAT_UTIL_TDD_THROWS_H
#define TEMPLAT_UTIL_TDD_THROWS_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019


#include <tuple>
#include <string>

#include "TempLat/util/demangle.h"
#include "TempLat/util/tdd/tddmacros.h"


namespace TempLat {


    /** \brief A standalone function for testing of an exception is properly thrown. Returns a bool and a description. */
    template <typename Exception, typename Lambda>
    std::pair<bool, std::string> Throws(Lambda ll) {
        bool success = false;
        try {
            ll();
        } catch (const Exception& e ) {
            success = true;
        } catch (Exception *e ) {
            success = true;
        }
        return {success, std::string("Must throw " + Demangle::demangle( typeid(Exception).name() ) + ".") };
    }
    
    /** \brief A helper for negating Throws. */
    template <typename Exception, typename Lambda>
    auto DoesNotThrow(Lambda ll) {
        auto result = Throws<Exception, Lambda>(ll);
        result.first = ! result.first;
        result.second.insert(4, " not");
        return result;
    }


    /** \brief A class which tests TempLat::Throws
      *
      * 
      * Unit test: make test-throws
      */

    class ThrowsTester {
    public:
    /* Put public methods here. These should change very little over time. */
        ThrowsTester() {

        }


    private:
    /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
        template <typename TestObjectUnknownHere>
        static inline void Test(TestObjectUnknownHere& tdd);
    };
}

#ifdef TEMPLATTEST
#include "TempLat/util/tdd/throws_test.h"
#endif


#endif
