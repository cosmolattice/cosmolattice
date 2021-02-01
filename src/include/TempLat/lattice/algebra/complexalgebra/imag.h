#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_IMAG_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_IMAG_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"


namespace TempLat {


    /** \brief A class which returns imaginary part of a fields or complex number.
     *
     * 
     * Unit test: make test-imag
     **/

    template< class T>
    static inline
    typename std::enable_if<HasComplexFieldGet<T>::value, decltype(std::declval<T>().ComplexFieldGet(std::declval<Tag<1>>()))>::type
    Imag(T&& t)
    {
        return t.ComplexFieldGet(Tag<1>());
    }

    template< class T>
    static inline
    typename std::enable_if<IsComplexType<T>::value, decltype(std::declval<T>().imag())>::type
    Imag(T&& t)
    {
        return t.imag();
    }

    struct ImagTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };





} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/imag_test.h"
#endif


#endif
