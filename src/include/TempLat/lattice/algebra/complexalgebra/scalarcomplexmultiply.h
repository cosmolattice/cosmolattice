#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_SCALARCOMPLEXFIELDMULTIPLY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldbinaryoperator.h"
#include "TempLat/lattice/algebra/helpers/hasstaticgetter.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat {

    /** \brief A class which implements scalar multiplication over complex numbers.
     *
     * 
     * 
     * Unit test: make test-scalarcomplexfieldmultiply
     **/

    template<typename R, typename T>
    class ScalarComplexFieldMultiply : public ComplexFieldBinaryOperator<R,T>{
    public:
        /* Put public methods here. These should change very little over time. */

        using ComplexFieldBinaryOperator<R,T>::mR;
        using ComplexFieldBinaryOperator<R,T>::mT;

        ScalarComplexFieldMultiply(const R& pR, const T& pT) :
                ComplexFieldBinaryOperator<R,T>(pR,pT){

        }

        auto ComplexFieldGet(Tag<0> t)
        {
            return mR * Real(mT);
        }
        auto ComplexFieldGet(Tag<1> t)
        {
            return mR * Imag(mT);
        }

        auto ComplexFieldGet(Tag<0> t, ptrdiff_t i)
        {
            return GetEval::getEval(mR,i) * mT.ComplexFieldGet(0_c,i);
        }
        auto ComplexFieldGet(Tag<1> t, ptrdiff_t i)
        {
            return GetEval::getEval(mR,i) * mT.ComplexFieldGet(1_c,i);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
        }

        std::string operatorString() const override
        {
            return "*";
        }

    private:


    };

    struct ScalarComplexFieldMultiplyTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    template <typename R, typename T>
    typename std::enable_if<!HasStaticGetter<R>::value && !IsComplexType<R>::value  && HasComplexFieldGet<T>::value, ScalarComplexFieldMultiply<R,T>>::type
    operator*(const R& r, const T& t)
    {
        return {r,t};
    }

    template <typename R, typename T>
    typename std::enable_if<!HasStaticGetter<T>::value  && !IsComplexType<T>::value && HasComplexFieldGet<R>::value, ScalarComplexFieldMultiply<T,R>>::type
    operator*(const R& r, const T& t)
    {
        return {t,r};
    }

    template <typename R, typename T>
    typename std::enable_if<!HasStaticGetter<T>::value  && !IsComplexType<T>::value && HasComplexFieldGet<R>::value, ScalarComplexFieldMultiply<T,R>>::type
    operator/(const R& r, const T& t)
    {
        return {1_c / t, r};
    }
} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/scalarcomplexmultiply_test.h"
#endif

#endif
