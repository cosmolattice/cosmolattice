#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2WRAPPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2operator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"


namespace TempLat {


    /** \brief A class which construct a SU2-like object.
     *
     * 
     * Unit test: make test-su2wrapper
     **/

    template <class A, class B, class C, class D>
    class SU2Wrapper : public SU2Operator {
    public:
        typedef typename GetGetReturnType<A>::type SV;

        /* Put public methods here. These should change very little over time. */
        SU2Wrapper(){

        }

        SU2Wrapper(const A& pA, const B& pB, const C& pC, const D& pD):
        mA(pA),mB(pB),mC(pC),mD(pD)
        {

        }

        auto SU2Get(Tag<0> t)
        {
            return mA;
            //return sqrt(1.0-Total(i,1,3,pow<2>(SU2Get(i))));
        }
        auto SU2Get(Tag<1> t)
        {
            return mB;
        }
        auto SU2Get(Tag<2> t)
        {
            return mC;
        }
        auto SU2Get(Tag<3> t)
        {
            return mD;
        }

        auto SU2Get(Tag<0> t, ptrdiff_t i)
        {
            return GetValue::get(mA,i);
            //return sqrt(1.0-Total(i,1,3,pow<2>(SU2Get(i))GetValue::get()),i);
        }
        auto SU2Get(Tag<1> t, ptrdiff_t i)
        {
            return GetValue::get(mB,i);
        }
        auto SU2Get(Tag<2> t, ptrdiff_t i)
        {
            return GetValue::get(mC,i);
        }
        auto SU2Get(Tag<3> t, ptrdiff_t i)
        {
            return GetValue::get(mD,i);
        }

        template<int N>
        auto operator()(Tag<N> t)
        {
            return SU2Get(t);
        }

        std::array<SV,4> SU2Get(ptrdiff_t i)
        {
            return {SU2Get(0_c,i), SU2Get(1_c,i), SU2Get(2_c,i), SU2Get(3_c,i)};
        }

        std::string toString() const{
            return "SU2("+GetString::get(mA)+","+GetString::get(mB)+","+GetString::get(mC)+","+GetString::get(mD)+")";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        A mA;
        B mB;
        C mC;
        D mD;


    public:
    };

    struct SU2WrapperTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <class A, class B, class C, class D>
    auto SU2Wrap(A&& pA, B&& pB, C&& pC, D&& pD)
    {
        return SU2Wrapper<A,B,C,D>(pA,pB,pC,pD);
    }

    template <typename F>
    auto SU2Wrap(F&& f)
    {
        return SU2Wrap(f(0_c), f(1_c), f(2_c), f(3_c));
    }

#define MakeSU2(a,expr) SU2Wrap([&](auto a){return expr;})



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2wrapper_test.h"
#endif


#endif
