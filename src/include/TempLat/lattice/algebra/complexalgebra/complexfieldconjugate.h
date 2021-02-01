#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDCONJUGATE_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDCONJUGATE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/real.h"
#include "TempLat/lattice/algebra/complexalgebra/imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"

namespace TempLat {


    /** \brief A class which computes the complex conjugate of a complex field.
     *
     * 
     * Unit test: make test-complexfieldconjugate
     **/

    template< typename R >
    class ComplexFieldConjugate : public ComplexFieldUnaryOperator<R> {
    public:

        using ComplexFieldUnaryOperator<R>::mR;

        /* Put public methods here. These should change very little over time. */
        ComplexFieldConjugate(const R& pR) : ComplexFieldUnaryOperator<R>(pR) {

        }

        auto ComplexFieldGet(Tag<0> t)
        {
            return Real(mR);
        }
        auto ComplexFieldGet(Tag<1> t)
        {
            return -Imag(mR);
        }

        auto ComplexFieldGet(Tag<0> t, ptrdiff_t i)
        {
            return mR.ComplexFieldGet(0_c,i);
        }
        auto ComplexFieldGet(Tag<1> t, ptrdiff_t i)
        {
            return -mR.ComplexFieldGet(1_c,i);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
        }

        std::string toString() const
        {
            return GetString::get(mR) + "^*";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */


    };

    struct ComplexFieldConjugateTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template < typename R >
    typename std::enable_if<HasComplexFieldGet<R>::value, ComplexFieldConjugate<R> >::type
    conj(const R& r)
    {
        return ComplexFieldConjugate<R>(r);
    }

    template < typename R >
    typename std::enable_if<HasComplexFieldGet<R>::value, ComplexFieldConjugate<R> >::type
    dagger(const R& r)
    {
        return ComplexFieldConjugate<R>(r);
    }

    OneType dagger(OneType)
    {
        return OneType();
    }
    OneType conj(OneType)
    {
        return OneType();
    }

    ZeroType conj(ZeroType t)
    {
        return t;
    };

    ZeroType dagger(ZeroType t)
    {
        return t;
    };





} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/complexfieldconjugate_test.h"
#endif


#endif
