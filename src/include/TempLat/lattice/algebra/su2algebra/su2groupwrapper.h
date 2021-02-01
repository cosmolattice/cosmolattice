#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2GROUPWRAPPER_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2GROUPWRAPPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/su2algebra/su2operator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/algebra/operators/power.h"

namespace TempLat {


    /** \brief A class which computes an element of the group SU(2).
     * By that, we mean that det=1 is imposed as a constrained on the 0th element, like in the SU2Fields.
     *
     * 
     * Unit test: make test-su2groupwrapper
     **/

    template <class A, class B, class C>
    class SU2GroupWrapper : public SU2Operator {
    public:
        typedef typename GetGetReturnType<A>::type SV;
        /* Put public methods here. These should change very little over time. */

        SU2GroupWrapper(const A& pA, const B& pB, const C& pC):
                mA(pA),mB(pB),mC(pC)
        {

        }

        auto SU2Get(Tag<0> t)
        {
            return sqrt(1.0-Total(i,1,3,pow<2>(SU2Get(i))));
        }
        auto SU2Get(Tag<1> t)
        {
            return mA;
        }
        auto SU2Get(Tag<2> t)
        {
            return mB;
        }
        auto SU2Get(Tag<3> t)
        {
            return mC;
        }

        auto SU2Get(Tag<0> t, ptrdiff_t i)
        {
            return cache[0];
            //return sqrt(1.0-Total(i,1,3,pow<2>(SU2Get(i))GetValue::get()),i);
        }
        auto SU2Get(Tag<1> t, ptrdiff_t i)
        {
            return cache[1];
        }
        auto SU2Get(Tag<2> t, ptrdiff_t i)
        {
            return cache[2];
        }
        auto SU2Get(Tag<3> t, ptrdiff_t i)
        {
            return cache[3];
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mA, i);
            DoEval::eval(mB, i);
            DoEval::eval(mC, i);

            cache[1] = GetValue::get(mA,i);
            cache[2] = GetValue::get(mB,i);
            cache[3] = GetValue::get(mC,i);
            cache[0] = sqrt(1.0 - pow<2>(cache[1]) - pow<2>(cache[2]) - pow<2>(cache[3]));
        }

        template<int N>
        auto operator()(Tag<N> t)
        {
            return SU2Get(t);
        }

        std::array<SV,4> SU2Get(ptrdiff_t i)
        {
            return std::move(cache);
        }

        std::string toString() const{
            return "SU2Group("+GetString::get(mA)+","+GetString::get(mB)+","+GetString::get(mC)+")";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        A mA;
        B mB;
        C mC;

        std::array<SV,4> cache;


    };

    struct SU2GroupWrapperTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <class A, class B, class C>
    auto SU2GroupWrap(A&& pA, B&& pB, C&& pC)
    {
        return SU2GroupWrapper<A,B,C>(pA,pB,pC);
    }

    template <class R>
    auto toSU2(R r){
        return SU2GroupWrap(r.SU2Get(1_c), r.SU2Get(2_c), r.SU2Get(3_c));
    }




} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/su2algebra/su2groupwrapper_test.h"
#endif


#endif
