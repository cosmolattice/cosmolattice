#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXFIELDGET_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_HELPERS_COMPLEXFIELDGET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): AdrienFlorio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tag.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/helpers/iscomplextype.h"



namespace TempLat {


    /** \brief A class which get real and imaginary part.
     *
     * 
     * Unit test: make test-complexfieldget
     **/

    class ComplexFieldGetter {
    public:

        template<typename R>
        static inline
        typename std::enable_if<IsComplexType<R>::value, decltype(real(std::declval<R>()))> get(R&& r, Tag<0> t)
        {
            return real(r);
        }

        template<typename R>
        static inline
        typename std::enable_if<IsComplexType<R>::value, decltype(imag(std::declval<R>()))> get(R&& r, Tag<1> t)
        {
            return imag(r);
        }

        template<typename R, int N>
        static inline
        typename std::enable_if<!IsComplexType<R>::value && HasComplexFieldGet<R>::value, decltype(std::declval<R>().ComplexFieldGet(std::declval<Tag<N>>()))>::type get(R&& r, Tag<N> t)
        {
            return r.ComplexFieldGet(t);
        }

        template<typename R>
        static inline
        typename std::enable_if<IsComplexType<R>::value, decltype(real(std::declval<R>()))> get(R&& r, Tag<0> t, ptrdiff_t i)
        {
            return real(r);
        }

        template<typename R>
        static inline
        typename std::enable_if<IsComplexType<R>::value, decltype(imag(std::declval<R>()))> get(R&& r, Tag<1> t, ptrdiff_t i)
        {
            return imag(r);
        }

        template<typename R, int N>
        static inline
        typename std::enable_if<!IsComplexType<R>::value && HasComplexFieldGet<R>::value, decltype(std::declval<R>().ComplexFieldGet(std::declval<Tag<N>>()))>::type get(R&& r, Tag<N> t, ptrdiff_t i)
        {
            return r.ComplexFieldGet(t,i);
        }

    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget_test.h"
#endif


#endif
