#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSHIFT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETSHIFT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/lattice/algebra/su2algebra/su2doubletunaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"



namespace TempLat {


    /** \brief A class which shifts a doublet in space.
     *
     *
     * Unit test: make test-su2shift
     **/

    template <typename R, int... N>
    class SU2DoubletShifter : public SU2DoubletUnaryOperator<R> {
    public:
        using SU2DoubletUnaryOperator<R>::mR;
        /* Put public methods here. These should change very little over time. */
        SU2DoubletShifter(const R& pR) : SU2DoubletUnaryOperator<R>(pR), shiftInd(shift<N...>(mR.SU2DoubletGet(0_c)).getShift()) {
            shiftString = shift<N...>(mR.SU2DoubletGet(0_c)).getString({N...}) ;
        }

        template<int M>
        auto SU2DoubletGet(Tag<M> t)
        {
            return shift<N...>(mR.SU2DoubletGet(t));
        }

        template<int M>
        auto SU2DoubletGet(Tag<M> t, ptrdiff_t i)
        {
            return mR.SU2DoubletGet(t,i+shiftInd);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i + shiftInd);
        }

        std::string operatorString() const override{
            return shiftString;
        }

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        ptrdiff_t shiftInd;
        std::string shiftString;


    };
    template <typename R, int N>
    class SU2DoubletShifterByOne : public SU2DoubletUnaryOperator<R> {
    public:
        using SU2DoubletUnaryOperator<R>::mR;
        /* Put public methods here. These should change very little over time. */
        SU2DoubletShifterByOne(const R& pR) : SU2DoubletUnaryOperator<R>(pR), shiftInd(shift<N>(mR.SU2DoubletGet(0_c)).getShift()) {

        }

        template<int M>
        auto SU2DoubletGet(Tag<M> t)
        {
            return shift<N>(mR.SU2DoubletGet(t));
        }

        template<int M>
        auto SU2DoubletGet(Tag<M> t, ptrdiff_t i)
        {
            return mR.SU2DoubletGet(t,i+shiftInd);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i + shiftInd);
        }

        std::string toString() const override{
            return GetString::get(mR)+"_(->"+std::to_string(N)+")";
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        ptrdiff_t shiftInd;

    };


    template <int... shifts, class R>
    typename std::enable_if<(sizeof...(shifts) > 1) && HasSU2DoubletGet<R>::value , SU2DoubletShifter<R,shifts...>>::type
    shift(const R& pR)
    {
        return SU2DoubletShifter<R,shifts...>(pR);
    }

    template <int N, class R>
    typename std::enable_if<HasSU2DoubletGet<R>::value, SU2DoubletShifterByOne<R,N>>::type
    shift(const R& pR)
    {
        return SU2DoubletShifterByOne<R,N>(pR);
    }
    template <int N, class R>
    typename std::enable_if<HasSU2DoubletGet<R>::value, SU2DoubletShifterByOne<R,N>>::type
    shift(const R& pR, Tag<N> t)
    {
        return SU2DoubletShifterByOne<R,N>(pR);
    }

    struct SU2DoubletShiftTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };


} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doubletshift_test.h"
#endif


#endif
