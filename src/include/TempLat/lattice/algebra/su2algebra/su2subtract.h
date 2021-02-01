#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SUBTRACT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SUBTRACT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/memory/memorytoolbox.h"


namespace TempLat {


    /** \brief A class which implements the SU2 substraction.
     *
     * 
     * Unit test: make test-su2sum
     **/

    template <typename R, typename T>
    class SU2Subtraction : public SU2BinaryOperator<R,T> {
    public:
        typedef typename SU2GetGetReturnType<R>::type SV;

        using SU2BinaryOperator<R,T>::mR;
        using SU2BinaryOperator<R,T>::mT;

        /* Put public methods here. These should change very little over time. */
        SU2Subtraction(const R& pR, const T& pT) :
                SU2BinaryOperator<R,T>(pR,pT){

        }

        template<int N>
        auto SU2Get(Tag<N> t)
        {
            return mR.SU2Get(t) - mT.SU2Get(t);
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

        template<int M>
        auto SU2Get(Tag<M> t, ptrdiff_t i)
        {
            return mR.SU2Get(t,i) - mT.SU2Get(t,i);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
        }

        virtual std::string operatorString() const {
            return "-";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */



    public:
    };

    struct SU2SubtractTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R, typename T>
    typename std::enable_if<HasSU2Get<R>::value && HasSU2Get<T>::value, SU2Subtraction<R,T>>::type
    operator-(const R& r, const T& t)
    {
        return {r,t};
    }

} /* TempLat */

#ifdef TEMPLATTEST
#include "su2subtract_test.h"
#endif


#endif
