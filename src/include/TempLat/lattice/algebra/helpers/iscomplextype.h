#ifndef TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_H
#define TEMPLAT_LATTICE_ALGEBRA_HELPERS_ISCOMPLEXTYPE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <complex>

#include "TempLat/util/tdd/tdd.h"

namespace TempLat {

    /** \brief SFINAE detection of std::complex<whatever>
     *
     * Unit test: make test-iscomplextype
     **/
    template <typename T>
    struct IsComplexType : public std::false_type {
    };

    template <typename T>
    struct IsComplexType<std::complex<T>> : public std::true_type {
    };

    template <typename T>
    struct IsComplexType<const std::complex<T>> : public std::true_type {
    };

    template <typename T>
    struct IsComplexType<std::complex<T>&> : public std::true_type {
    };

    template <typename T>
    struct IsComplexType<const std::complex<T>&> : public std::true_type {
    };

    /** \brief a mini tester class... */
    struct IsComplexTypeTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/helpers/iscomplextype_test.h"
#endif


#endif
