#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DAGGER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DAGGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2unaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"



namespace TempLat {


    /** \brief A class which implements the hermitian conjugation.
     *
     * 
     * Unit test: make test-su2dagger
     **/

    template<typename R>
    class SU2Dagger : public SU2UnaryOperator<R> {
    public:

        typedef typename SU2GetGetReturnType<R>::type SV;
        using SU2UnaryOperator<R>::mR;

        /* Put public methods here. These should change very little over time. */
        SU2Dagger(const R& pR): SU2UnaryOperator<R>(pR) {

        }

        auto SU2Get(Tag<0> t)
        {
            return  mR.SU2Get(0_c);
        }
        auto SU2Get(Tag<1> t)
        {
            return  - mR.SU2Get(1_c);
        }
        auto SU2Get(Tag<2> t)
        {
            return  - mR.SU2Get(2_c);
        }
        auto SU2Get(Tag<3> t)
        {
            return  - mR.SU2Get(3_c);
        }

        template<int N>
        auto operator()(Tag<N> t)
        {
            return SU2Get(t);
        }

        auto SU2Get(Tag<0> t, ptrdiff_t i)
        {
            return  mR.SU2Get(0_c, i);
        }
        auto SU2Get(Tag<1> t, ptrdiff_t i)
        {
            return  - mR.SU2Get(1_c, i);
        }
        auto SU2Get(Tag<2> t, ptrdiff_t i)
        {
            return  - mR.SU2Get(2_c, i);
        }
        auto SU2Get(Tag<3> t, ptrdiff_t i)
        {
            return  - mR.SU2Get(3_c, i);
        }

        std::array<SV,4> SU2Get(ptrdiff_t i)
        {
            return {SU2Get(0_c,i), SU2Get(1_c,i), SU2Get(2_c,i), SU2Get(3_c,i)};
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i);
        }

       std::string toString() const override {

            return  GetString::get(mR)+ "^\u2020";
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
    };

    struct SU2DaggerTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template < class R >
    typename std::enable_if<HasSU2Get<R>::value, SU2Dagger<R> >::type
    dagger(const R& r)
    {
        return SU2Dagger<R>(r);
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "su2dagger_test.h"
#endif


#endif
