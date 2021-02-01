#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_REAL_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"

namespace TempLat {


    /** \brief A class which get real parts of fields.
     *
     *
     * 
     * Unit test: make test-real
     **/


    template< class T>
    static inline
    typename std::enable_if<HasComplexFieldGet<T>::value, decltype(std::declval<T>().ComplexFieldGet(std::declval<Tag<0>>()))>::type
    Real(T&& t)
    {
                return t.ComplexFieldGet(0_c);
    }

    template< class T>
    static inline
    typename std::enable_if<IsComplexType<T>::value, decltype(std::declval<T>().real())>::type
    Real(T&& t)
    {
        return t.real();
    }


    struct RealTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/real_test.h"
#endif


#endif
