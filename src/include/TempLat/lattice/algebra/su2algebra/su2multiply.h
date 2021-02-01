#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2MULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2MULTIPLY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/su2binaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/rangeiteration/for_in_range.h"


namespace TempLat {


    /** \brief A class which
     *  implement SU(2) matrix multiplication.
     * 
     * 
     * Unit test: make test-su2multiply
     **/

    template<typename R, typename T>
    class SU2Multiplication : public SU2BinaryOperator<R,T> {
    public:
        typedef typename SU2GetGetReturnType<R>::type SV;
        using SU2BinaryOperator<R,T>::mR;
        using SU2BinaryOperator<R,T>::mT;

        /* Put public methods here. These should change very little over time. */
        SU2Multiplication(const R& pR, const T& pT): SU2BinaryOperator<R,T>(pR,pT) {

        }

        auto SU2Get(Tag<0> t)
        {
            return mR.SU2Get(0_c) * mT.SU2Get(0_c) - mR.SU2Get(1_c) * mT.SU2Get(1_c) - mR.SU2Get(2_c) * mT.SU2Get(2_c) - mR.SU2Get(3_c) * mT.SU2Get(3_c);
        }
        auto SU2Get(Tag<1> t)
        {
            return mR.SU2Get(0_c) * mT.SU2Get(1_c) + mR.SU2Get(3_c) * mT.SU2Get(2_c) + mR.SU2Get(1_c) * mT.SU2Get(0_c) - mR.SU2Get(2_c) * mT.SU2Get(3_c);
        }
        auto SU2Get(Tag<2> t)
        {
            return mR.SU2Get(0_c) * mT.SU2Get(2_c) + mR.SU2Get(2_c) * mT.SU2Get(0_c) + mR.SU2Get(1_c) * mT.SU2Get(3_c) - mR.SU2Get(3_c) * mT.SU2Get(1_c);
        }
        auto SU2Get(Tag<3> t)
        {
            return mR.SU2Get(0_c) * mT.SU2Get(3_c) + mR.SU2Get(3_c) * mT.SU2Get(0_c) + mR.SU2Get(2_c) * mT.SU2Get(1_c) - mR.SU2Get(1_c) * mT.SU2Get(2_c);
        }


        auto SU2Get(ptrdiff_t i)
        {
            return std::move(cache);
        }
        template <int N>
        auto SU2Get(Tag<N> t, ptrdiff_t i)
        {
            return cache[N];
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);
            ForLoop(j,0,3,
                    cL[j] = this->mR.SU2Get(j,i);
            cR[j] = this->mT.SU2Get(j,i);
                    );
            cache[0] = cL[0] * cR[0] - cL[1] *cR[1] - cL[2] *cR[2] - cL[3] *cR[3];
            cache[1] = cL[0] * cR[1] + cL[1] *cR[0] + cL[3] *cR[2] - cL[2] *cR[3];
            cache[2] = cL[0] * cR[2] + cL[2] *cR[0] + cL[1] *cR[3] - cL[3] *cR[1];
            cache[3] = cL[0] * cR[3] + cL[3] *cR[0] + cL[2] *cR[1] - cL[1] *cR[2];
        }

        virtual std::string operatorString() const {
            return ".";
        }

    private:


        std::array<SV,4> cL;
        std::array<SV,4> cR;
        std::array<SV,4> cache;
    };

    struct SU2MultiplyTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename R, typename T>
    typename std::enable_if<HasSU2Get<R>::value && HasSU2Get<T>::value, SU2Multiplication<R,T>>::type
    operator*(const R& r, const T& t)
    {
        return {r,t};
    }

    template < typename T>
    typename std::enable_if<HasSU2Get<T>::value, T>::type
    operator*(OneType r, const T& t)
    {
        return t;
    }

    template <typename R>
    typename std::enable_if<HasSU2Get<R>::value , R>::type
    operator*(const R& r, OneType t)
    {
        return r;
    }

    template < typename T>
    typename std::enable_if< HasSU2Get<T>::value, ZeroType>::type
    operator*(ZeroType r, const T& t)
    {
        return r;
    }

    template <typename R>
    typename std::enable_if<HasSU2Get<R>::value, ZeroType>::type
    operator*(const R& r, ZeroType t)
    {
        return t;
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/su2algebra/su2multiply_test.h"
#endif


#endif
