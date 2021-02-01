#ifndef COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2DOUBLETMULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2DOUBLETMULTIPLY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldmultiply.h"

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"


namespace TempLat {


    /** \brief A class which implements the fundamental action of SU2.
     *
     * 
     * Unit test: make test-complexfieldsu2multiply
     **/


    template < typename R, typename T >
    class ComplexFieldSU2DoubletMultiplication : public SU2DoubletBinaryOperator<R,T> {
    public:

        using SU2DoubletBinaryOperator<R,T>::mR;
        using SU2DoubletBinaryOperator<R,T>::mT;

        /* Put public methods here. These should change very little over time. */
        ComplexFieldSU2DoubletMultiplication(const R& pR, const T& pT) : SU2DoubletBinaryOperator<R,T>(pR,pT) {

        }

        auto SU2DoubletGet(Tag<0> t)
        {
            return Real(mR * Complexify(mT.SU2DoubletGet(0_c),mT.SU2DoubletGet(1_c)));
        }
        auto SU2DoubletGet(Tag<1> t)
        {
            return Imag(mR * Complexify(mT.SU2DoubletGet(0_c),mT.SU2DoubletGet(1_c)));
        }
        auto SU2DoubletGet(Tag<2> t)
        {
            return Real(mR * Complexify(mT.SU2DoubletGet(2_c),mT.SU2DoubletGet(3_c)));
        }
        auto SU2DoubletGet(Tag<3> t)
        {
            return Imag(mR * Complexify(mT.SU2DoubletGet(2_c),mT.SU2DoubletGet(3_c)));
        }

        auto SU2DoubletGet(Tag<0> t, ptrdiff_t i)
        {
            return mR.ComplexFieldGet(0_c,i) * mT.SU2DoubletGet(0_c,i) - mR.ComplexFieldGet(1_c,i) * mT.SU2DoubletGet(1_c,i);
        }
        auto SU2DoubletGet(Tag<1> t, ptrdiff_t i)
        {
            return mR.ComplexFieldGet(0_c,i) * mT.SU2DoubletGet(1_c,i) + mR.ComplexFieldGet(1_c,i) * mT.SU2DoubletGet(0_c,i);

        }
        auto SU2DoubletGet(Tag<2> t, ptrdiff_t i)
        {
            return mR.ComplexFieldGet(0_c,i) * mT.SU2DoubletGet(2_c,i) - mR.ComplexFieldGet(1_c,i) * mT.SU2DoubletGet(3_c,i);

        }
        auto SU2DoubletGet(Tag<3> t, ptrdiff_t i)
        {
            return mR.ComplexFieldGet(0_c,i) * mT.SU2DoubletGet(3_c,i) + mR.ComplexFieldGet(1_c,i) * mT.SU2DoubletGet(2_c,i);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
        }

        std::string operatorString() const {
            return "*";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    };

    struct ComplexFieldSU2DoubletMultiplyTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R, typename T>
    typename std::enable_if<HasComplexFieldGet<R>::value && HasSU2DoubletGet<T>::value, ComplexFieldSU2DoubletMultiplication<R,T>>::type
    operator*(const R& r, const T& t)
    {
        return {r,t};
    }

    template <typename R, typename T>
    typename std::enable_if<HasComplexFieldGet<T>::value && HasSU2DoubletGet<R>::value, ComplexFieldSU2DoubletMultiplication<T,R>>::type
    operator*(const R& r, const T& t)
    {
        return {t,r};
    }

    template <typename T>
    typename std::enable_if< HasSU2DoubletGet<T>::value, ComplexFieldSU2DoubletMultiplication<ComplexFieldWrapper<double,ZeroType>,T>>::type
    operator*(double r, const T& t)
    {
        return Complexify(r,ZeroType()) * t;
    }

    template <typename T>
    typename std::enable_if< HasSU2DoubletGet<T>::value, ComplexFieldSU2DoubletMultiplication<ComplexFieldWrapper<float,ZeroType>,T>>::type
    operator*(float r, const T& t)
    {
        return Complexify(r,ZeroType()) * t;
    }

    template <typename T>
    typename std::enable_if< HasSU2DoubletGet<T>::value, ComplexFieldSU2DoubletMultiplication<ComplexFieldWrapper<double,ZeroType>,T>>::type
    operator/(const T& t, double r)
    {
        return Complexify(1_c/r,ZeroType()) * t;
    }

    template <typename T>
    typename std::enable_if< HasSU2DoubletGet<T>::value, ComplexFieldSU2DoubletMultiplication<ComplexFieldWrapper<float,ZeroType>,T>>::type
    operator/(const T& t, float r)
    {
        return Complexify(1_c/r,ZeroType()) * t;
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "complexfieldsu2doubletmultiply_test.h"
#endif


#endif
