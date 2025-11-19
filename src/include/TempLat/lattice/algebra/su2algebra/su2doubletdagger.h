#ifndef TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_H
#define TEMPLAT_LATTICE_ALGEBRA_SU2ALGEBRA_SU2DOUBLETDAGGER_H

/*  This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s):  Adrien Florio, Year: 2025

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletunaryoperator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletgetgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat {


    /** \brief A class which compute the hermitean conjugate of Doublets
     *
     * 
     * Unit test: make test-su2doubletdagger
     **/

    template<typename R>
    class SU2DoubletDagger : public SU2DoubletUnaryOperator<R> {
    public:

        typedef typename SU2DoubletGetGetReturnType<R>::type SV;
        using SU2DoubletUnaryOperator<R>::mR;

        /* Put public methods here. These should change very little over time. */
        SU2DoubletDagger(const R& pR): SU2DoubletUnaryOperator<R>(pR) {

        }

        auto SU2DoubletGet(Tag<0> t)
        {
            return  mR.SU2DoubletGet(0_c);
        }
        auto SU2DoubletGet(Tag<1> t)
        {
            return  - mR.SU2DoubletGet(1_c);
        }
        auto SU2DoubletGet(Tag<2> t)
        {
            return  mR.SU2DoubletGet(2_c);
        }
        auto SU2DoubletGet(Tag<3> t)
        {
            return  - mR.SU2DoubletGet(3_c);
        }

        template<int N>
        auto operator()(Tag<N> t)
        {
            return SU2DoubletGet(t);
        }

        auto SU2DoubletGet(Tag<0> t, ptrdiff_t i)
        {
            return  mR.SU2DoubletGet(0_c, i);
        }
        auto SU2DoubletGet(Tag<1> t, ptrdiff_t i)
        {
            return  - mR.SU2DoubletGet(1_c, i);
        }
        auto SU2DoubletGet(Tag<2> t, ptrdiff_t i)
        {
            return  mR.SU2DoubletGet(2_c, i);
        }
        auto SU2DoubletGet(Tag<3> t, ptrdiff_t i)
        {
            return  - mR.SU2DoubletGet(3_c, i);
        }

        std::array<SV,4> SU2DoubletGet(ptrdiff_t i)
        {
            return {SU2DoubletGet(0_c,i), SU2DoubletGet(1_c,i), SU2DoubletGet(2_c,i), SU2DoubletGet(3_c,i)};
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


    template < class R >
    typename std::enable_if<HasSU2DoubletGet<R>::value, SU2DoubletDagger<R> >::type
    dagger(const R& r)
    {
        return SU2DoubletDagger<R>(r);
    };

        struct SU2DoubletDaggerTester{
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };



} /* TempLat */
#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/su2algebra/su2doubletdagger_test.h"
#endif

#endif
