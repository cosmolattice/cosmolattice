#ifndef TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_H
#define TEMPLAT_LATTICE_ALGEBRA_SPATIALDERIVATIVES_NORMGRADIENTSQUARE_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/shiftedaccessor.h"

#include "TempLat/lattice/algebra/spatialderivatives/latticeforwardgradient.h"
#include "TempLat/lattice/algebra/operators/unaryoperator.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"



namespace TempLat {


    /** \brief A class which computes the gradient square norm.
     *
     * 
     * Unit test: make test-normgradientsquare
     **/
    template<int nDimensions, typename R>
    class NormGradientSquare : public UnaryOperator<R> {
    public:
        typedef typename GetGetReturnType<R>::type SV;
        typedef typename GetFloatType<SV>::type S;
        using UnaryOperator<R>::mR;

        /* Put public methods here. These should change very little over time. */
        NormGradientSquare(const R& pR) :
        UnaryOperator<R>(pR), dx2(pow<2>(GetDx::getDx(pR)))
        {
            fixGradientMap(GetJumps::apply(mR));
        }


        auto get(ptrdiff_t i)
        {
            S res = 0;
            for(size_t j = 0 ; j < nDimensions; ++j)
            {
                res += pow<2>( mR.get(i+jumps[j])-mR.get(i) ) / dx2;
            }
            return res;
        }

        std::string toString() const{
            return "|Grad(" + GetString::get(mR)+")|^2";
        }

        void doWeNeedGhosts() override
        {
            mR.confirmGhostsUpToDate();
        }

        void eval(ptrdiff_t i) override
        {
            DoEval::eval(mR, i);
            for(size_t j=0; j<nDimensions;++j) DoEval::eval(mR, i + jumps[j]);
        }


        template <typename S>
        inline
        auto d(const S& other) {
            return 2 * LatForwardGrad(mR) * LatForwardGrad(mR.d(other));
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        std::array<ptrdiff_t,nDimensions> jumps;
        S dx2;

        void fixGradientMap(const JumpsHolder& jump) {
            for ( ptrdiff_t dim = 0; dim < nDimensions; ++dim) {
                jumps[dim]=pushBack(dim, 1, jump);
            }
        }

        ptrdiff_t pushBack(ptrdiff_t dim, ptrdiff_t dSign, const JumpsHolder& jump) {
            std::vector<ptrdiff_t> shifts(nDimensions, (ptrdiff_t) 0);
            shifts[dim] = dSign;

            return ShiftedAccessor(jump,shifts).getJump();
        }
    };

    class NormGradientSquareTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };

    template<int nDimensions=3, typename R>
    typename std::enable_if< HasGetMethod<R>::value, NormGradientSquare<nDimensions,R> >::type
    Grad2(R pR)
    {
        return NormGradientSquare<nDimensions,R>(pR);
    }


} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/spatialderivatives/normgradientsquare_test.h"
#endif


#endif
