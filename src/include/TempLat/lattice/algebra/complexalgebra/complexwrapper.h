#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXWRAPPER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXWRAPPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "real.h"
#include "imag.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldoperator.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"



namespace TempLat {


    /** \brief A class which wraps two objects as a complex field.
     *
     * 
     * Unit test: make test-complexwrapper
     **/

    template < class R, class T >
    class ComplexFieldWrapper : public ComplexFieldOperator{
    public:
        /* Put public methods here. These should change very little over time. */
        ComplexFieldWrapper() {

        }

        ComplexFieldWrapper(const R& pR, const T& pT) :
                mR(pR),
                mT(pT){

        }

        auto ComplexFieldGet(Tag<0> t)
        {
            return mR;
        }
        auto ComplexFieldGet(Tag<1> t)
        {
            return mT;
        }

        template<int N>
        auto operator()(Tag<N> t) {
            return ComplexFieldGet(t);
        }

        auto ComplexFieldGet(Tag<0> t, ptrdiff_t i)
        {
            return GetEval::getEval(mR,i);
        }
        auto ComplexFieldGet(Tag<1> t, ptrdiff_t i)
        {
            return GetEval::getEval(mT,i);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
        }

        std::string toString() const{
            return "Complex("+GetString::get(mR)+","+GetString::get(mT)+")";
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        R mR;
        T mT;



    };

    struct ComplexFieldWrapperTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R, typename T>
    ComplexFieldWrapper<R,T> Complexify(const R& r, const T& t)
    {
        return {r,t};
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper_test.h"
#endif


#endif
