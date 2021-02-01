#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUM_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSUM_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat {


    /** \brief A class which implements the sum of 2 su2 doublets.
     *
     * 
     * Unit test: make test-su2doubletsum
     **/

    template <typename R, typename T>
    class SU2DoubletAddition : public SU2DoubletBinaryOperator<R,T> {
    public:
        /* Put public methods here. These should change very little over time. */

        using SU2DoubletBinaryOperator<R,T>::mR;
        using SU2DoubletBinaryOperator<R,T>::mT;

        SU2DoubletAddition(const R& pR, const T& pT) :
                SU2DoubletBinaryOperator<R,T>(pR,pT){

        }

        template<int N>
        auto SU2DoubletGet(Tag<N> t)
        {
            return mT.SU2DoubletGet(t) + mR.SU2DoubletGet(t);
        }

        template<int N>
        auto SU2DoubletGet(Tag<N> t, ptrdiff_t i)
        {
            return mT.SU2DoubletGet(t, i) + mR.SU2DoubletGet(t, i);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
        }

        std::string operatorString() const override
        {
            return "+";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

    public:
    };

    struct SU2DoubletSumTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R, typename T>
    typename std::enable_if<HasSU2DoubletGet<R>::value && HasSU2DoubletGet<T>::value, SU2DoubletAddition<R,T>>::type
    operator+(const R& r, const T& t)
    {
        return {r,t};
    }



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doubletsum_test.h"
#endif


#endif
