#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_U1WRAPPER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_U1WRAPPER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"


namespace TempLat {


    /** \brief Exponential map for U(1).
     *
     * 
     * Unit test: make test-u1wrapper
     **/

    template <typename R>
    class U1Exponential : public ComplexFieldUnaryOperator<R>{
    public:
        typedef typename GetGetReturnType<R>::type SV;
        /* Put public methods here. These should change very little over time. */
        using ComplexFieldUnaryOperator<R>::mR;

        U1Exponential(const R& pR) :
                ComplexFieldUnaryOperator<R>(pR){

            }

        auto ComplexFieldGet(Tag<0> t)
        {
            return cos(mR);
        }
        auto ComplexFieldGet(Tag<1> t)
        {
            return sin(mR);
        }

        auto ComplexFieldGet(Tag<0> t, ptrdiff_t i)
        {
            return cacheRe;
         //   return std::cos(GetEval::getEval(mR,i));
        }
        auto ComplexFieldGet(Tag<1> t, ptrdiff_t i)
        {
            return cacheIm;
           // return std::sin(GetEval::getEval(mR,i));
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            SV tmp = GetEval::getEval(mR,i);
            cacheRe = std::cos(tmp);
            cacheIm = std::sin(tmp);
        }

        std::string toString() const{
            return "U1("+GetString::get(mR)+")";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        SV cacheRe;
        SV cacheIm;

    };

    struct U1ExponentialTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


    template<typename R>
    auto complexPhase(R&& r)
    {
        return U1Exponential<R>(std::forward<R>(r));
    }

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/gaugealgebra/u1exponential_test.h"
#endif


#endif
