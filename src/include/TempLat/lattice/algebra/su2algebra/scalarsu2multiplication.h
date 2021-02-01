#ifndef COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2MULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_COMPLEXFIELDSU2MULTIPLY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldmultiply.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"

namespace TempLat {


    /** \brief A class which multiplies SU2 field by complex fields.
     *
     * 
     * Unit test: make test-complexfieldsu2multiply
     **/
    template < typename R, typename T >
    class ScalarSU2Multiplication : public SU2BinaryOperator<R,T> {
    public:

        using SU2BinaryOperator<R,T>::mR;
        using SU2BinaryOperator<R,T>::mT;

        /* Put public methods here. These should change very little over time. */
        ScalarSU2Multiplication(const R& pR, const T& pT) : SU2BinaryOperator<R,T>(pR,pT) {

        }


        typedef typename SU2GetGetReturnType<T>::type SV;


        auto SU2Get(Tag<0> t)
        {
            return mR * mT.SU2Get(0_c);
        }
        auto SU2Get(Tag<1> t)
        {
            return mR * mT.SU2Get(1_c);
        }
        auto SU2Get(Tag<2> t)
        {
            return mR * mT.SU2Get(2_c);
        }
        auto SU2Get(Tag<3> t)
        {
            return mR * mT.SU2Get(3_c);
        }

        auto SU2Get(Tag<0> t, ptrdiff_t i)
        {
            return GetValue::get(mR,i) * mT.SU2Get(0_c,i);
        }
        auto SU2Get(Tag<1> t, ptrdiff_t i)
        {
            return GetValue::get(mR,i) * mT.SU2Get(1_c,i);
        }
        auto SU2Get(Tag<2> t, ptrdiff_t i)
        {
            return GetValue::get(mR,i) * mT.SU2Get(2_c,i);
        }
        auto SU2Get(Tag<3> t, ptrdiff_t i)
        {
            return GetValue::get(mR,i) * mT.SU2Get(3_c,i);
        }

        std::array<SV,4> SU2Get(ptrdiff_t i)
        {
            return {SU2Get(0_c,i), SU2Get(1_c,i), SU2Get(2_c,i), SU2Get(3_c,i)};
        }

        template<int N>
        auto operator()(Tag<N> t)
        {
            return SU2Get(t);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
        }
        virtual std::string operatorString() const {
            return "*";
        }




    };

    struct ComplexFieldSU2MultiplyTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

   /* template <typename R, typename T>
    typename std::enable_if<HasComplexFieldGet<R>::value && HasSU2Get<T>::value, ComplexFieldSU2Multiplication<R,T>>::type
    operator*(const R& r, const T& t)
    {
        return {r,t};
    }

    template <typename R, typename T>
    typename std::enable_if<HasComplexFieldGet<T>::value && HasSU2Get<R>::value, ComplexFieldSU2Multiplication<T,R>>::type
    operator*(const R& r, const T& t)
    {
        return {t,r};
    }*/

    template <typename T>
    typename std::enable_if< HasSU2Get<T>::value, ScalarSU2Multiplication<double,T>>::type
    operator*(double r, const T& t)
    {
        return {r,t};
    }

    template <typename T>
    typename std::enable_if< HasSU2Get<T>::value, ScalarSU2Multiplication<float,T>>::type
    operator*(float r, const T& t)
    {
        return {r,t};
    }




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/su2algebra/scalarsu2multiplication_test.h"
#endif


#endif
