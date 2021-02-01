#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_ASFOURIER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_ASFOURIER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"


namespace TempLat {


    /** \brief A class which treats a complex field as an object in fourier space.
     *
     * 
     * Unit test: make test-asfourier
     **/

    template<typename R>
    class ComplexFieldAsFourier {
    public:
        using mRType = typename ComplexGetGetReturnType<R>::type;
        /* Put public methods here. These should change very little over time. */
        ComplexFieldAsFourier(const R& pR) : mR(pR){

        }


        auto get(ptrdiff_t i)
        {
            return std::complex<mRType>(mR.ComplexFieldGet(0_c).get(i),mR.ComplexFieldGet(1_c).get(i));
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    R mR;


    };

    struct AsFourierTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R>
    ComplexFieldAsFourier<R> asFourier(R&& r)
    {
        return ComplexFieldAsFourier<R>(std::forward<R>(r));
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/asfourier_test.h"
#endif


#endif
