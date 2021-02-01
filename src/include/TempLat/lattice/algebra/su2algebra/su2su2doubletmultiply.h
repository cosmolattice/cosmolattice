#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SU2DOUBLETMULTIPLY_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SU2DOUBLETMULTIPLY_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletbinaryoperator.h"
#include "TempLat/util/rangeiteration/for_in_range.h"


namespace TempLat {


    /** \brief A class which implemetns SU2 SU2Doublet multiplication.
     *
     * 
     * Unit test: make test-su2su2doubletmultiply
     **/

    template < class R, class T >
    class SU2SU2DoubletMultiplication : public SU2DoubletBinaryOperator<R,T> {
    public:
        using SU2DoubletBinaryOperator<R,T>::mR;
        using SU2DoubletBinaryOperator<R,T>::mT;

        typedef typename SU2GetGetReturnType<R>::type SV;

        /* Put public methods here. These should change very little over time. */
        SU2SU2DoubletMultiplication(const R& pR, const T& pT): SU2DoubletBinaryOperator<R,T>(pR,pT) {

        }

        auto SU2DoubletGet(Tag<0> t)
        {
            return mR.SU2Get(0_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(2_c) * mT.SU2DoubletGet(2_c) - mR.SU2Get(3_c) * mT.SU2DoubletGet(1_c) - mR.SU2Get(1_c) * mT.SU2DoubletGet(3_c);
        }
        auto SU2DoubletGet(Tag<1> t)
        {
            return mR.SU2Get(0_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(3_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(2_c) * mT.SU2DoubletGet(3_c) + mR.SU2Get(1_c) * mT.SU2DoubletGet(2_c);
        }
        auto SU2DoubletGet(Tag<2> t)
        {
            return - mR.SU2Get(1_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(3_c) * mT.SU2DoubletGet(3_c) - mR.SU2Get(2_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(0_c) * mT.SU2DoubletGet(2_c);
        }
        auto SU2DoubletGet(Tag<3> t)
        {
            return - mR.SU2Get(2_c) * mT.SU2DoubletGet(1_c) + mR.SU2Get(1_c) * mT.SU2DoubletGet(0_c) + mR.SU2Get(0_c) * mT.SU2DoubletGet(3_c) - mR.SU2Get(3_c) * mT.SU2DoubletGet(2_c);
        }

        template <int N>
        auto SU2DoubletGet(Tag<N> t, ptrdiff_t i)
        {
            return cache[N];
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
            DoEval::eval(mT, i);

            ForLoop(j,0,3,
                    this->cL[j] = this->mR.SU2Get(j,i);
                            this->cR[j] = this->mT.SU2DoubletGet(j,i);
            );


            cache[0] = cL[0] * cR[0] + cL[2] *cR[2] - cL[3] *cR[1] - cL[1] *cR[3];
            cache[1] = cL[0] * cR[1] + cL[3] *cR[0] + cL[2] *cR[3] + cL[1] *cR[2];
            cache[2] = - cL[1] * cR[1] + cL[3] *cR[3] - cL[2] *cR[0] + cL[0] *cR[2];
            cache[3] = - cL[2] * cR[1] + cL[1] *cR[0] + cL[0] *cR[3] - cL[3] *cR[2];
        }

        virtual std::string operatorString() const {
            return "*";
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::array<SV,4> cL;
        std::array<SV,4> cR;
        std::array<SV,4> cache;

    };


        struct SU2SU2DoubletMultiplyTester{
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };

    template <typename R, typename T>
    typename std::enable_if<HasSU2Get<R>::value && HasSU2DoubletGet<T>::value, SU2SU2DoubletMultiplication<R,T>>::type
    operator*(const R& r, const T& t)
    {
        return {r,t};
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "su2su2doubletmultiply_test.h"
#endif


#endif
