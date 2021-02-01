 #ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETWRAPPER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETWRAPPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletoperator.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"

namespace TempLat {


    /** \brief A class which creates a doublet out of some preexisting objects
     *
     * 
     * Unit test: make test-su2doubletwrapper
     **/

    template <class A, class B, class C, class D>
    class SU2DoubletWrapper : public SU2DoubletOperator {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2DoubletWrapper(){

        }


        SU2DoubletWrapper(const A& pA, const B& pB, const C& pC, const D& pD):
                mA(pA),mB(pB),mC(pC),mD(pD)
        {

        }

        auto SU2DoubletGet(Tag<0> t)
        {
            return mA;
        }
        auto SU2DoubletGet(Tag<1> t)
        {
            return mB;
        }
        auto SU2DoubletGet(Tag<2> t)
        {
            return mC;
        }
        auto SU2DoubletGet(Tag<3> t)
        {
            return mD;
        }

        auto SU2DoubletGet(Tag<0> t, ptrdiff_t i)
        {
            return GetValue::get(mA,i);
            //return sqrt(1.0-Total(i,1,3,pow<2>(SU2DoubletGet(i))GetValue::get()),i);
        }
        auto SU2DoubletGet(Tag<1> t, ptrdiff_t i)
        {
            return GetValue::get(mB,i);
        }
        auto SU2DoubletGet(Tag<2> t, ptrdiff_t i)
        {
            return GetValue::get(mC,i);
        }
        auto SU2DoubletGet(Tag<3> t, ptrdiff_t i)
        {
            return GetValue::get(mD,i);
        }

        template<int N>
        auto operator()(Tag<N> t) {
            return SU2DoubletGet(t);
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

    struct SU2DoubletWrapperTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <class A, class B, class C, class D>
    auto SU2DoubletWrap(A&& pA, B&& pB, C&& pC, D&& pD)
    {
        return SU2DoubletWrapper<A,B,C,D>(pA,pB,pC,pD);
    }

    template <typename F>
    auto SU2DoubletWrap(F&& f)
    {
        return SU2DoubletWrap(f(0_c), f(1_c), f(2_c), f(3_c));
    }

#define MakeSU2Doublet(a,expr) SU2DoubletWrap([&](auto a){return expr;})



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doubletwrapper_test.h"
#endif


#endif
