#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2SHIFT_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2SHIFT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
#include "TempLat/lattice/algebra/su2algebra/su2operator.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/su2algebra/su2unaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"


namespace TempLat {


    /** \brief A class which aaplies the shift method to su2 objects.
     *
     * 
     * Unit test: make test-su2shift
     **/

    template <typename R, int... N>
    class SU2Shifter : public SU2UnaryOperator<R> {
    public:
        typedef typename SU2GetGetReturnType<R>::type SV;
        using SU2UnaryOperator<R>::mR;

        /* Put public methods here. These should change very little over time. */
        SU2Shifter(const R& pR) : SU2UnaryOperator<R>(pR), shiftInd(shift<N...>(mR.SU2Get(0_c)).getShift()) {
            shiftString = shift<N...>(mR.SU2Get(0_c)).getString({N...}) ;
        }

        template<int M>
        auto SU2Get(Tag<M> t)
        {
            return shift<N...>(mR.SU2Get(t));
        }
        template<int M>
        auto SU2Get(Tag<M> t, ptrdiff_t i)
        {
            return mR.SU2Get(t,i+shiftInd);
        }

        std::array<SV,4> SU2Get(ptrdiff_t i)
        {
            return {SU2Get(0_c,i), SU2Get(1_c,i), SU2Get(2_c,i), SU2Get(3_c,i)};
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
    class SU2ShifterByOne : public SU2UnaryOperator<R> {
    public:
        typedef typename SU2GetGetReturnType<R>::type SV;
        using SU2UnaryOperator<R>::mR;

        /* Put public methods here. These should change very little over time. */
        SU2ShifterByOne(const R& pR) : SU2UnaryOperator<R>(pR), shiftInd(shift<N>(mR.SU2Get(1_c)).getShift()) {

        }

        template<int M>
        auto SU2Get(Tag<M> t)
        {
            return shift<N>(mR.SU2Get(t));
        }

        template<int M>
        auto SU2Get(Tag<M> t, ptrdiff_t i)
        {
            return mR.SU2Get(t,i+shiftInd);
        }

        template<int Q>
        auto operator()(Tag<Q> t)
        {
            return SU2Get(t);
        }

        std::array<SV,4> SU2Get(ptrdiff_t i)
        {
            return {SU2Get(0_c,i), SU2Get(1_c,i), SU2Get(2_c,i), SU2Get(3_c,i)};
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
    typename std::enable_if<(sizeof...(shifts) > 1) && HasSU2Get<R>::value , SU2Shifter<R,shifts...>>::type
    shift(const R& pR)
    {
        return SU2Shifter<R,shifts...>(pR);
    }

    template <int N, class R>
    typename std::enable_if<HasSU2Get<R>::value, SU2ShifterByOne<R,N>>::type
    shift(const R& pR)
    {
        return SU2ShifterByOne<R,N>(pR);
    }

    template < class R, int N>
    typename std::enable_if<HasSU2Get<R>::value, SU2ShifterByOne<R,N>>::type
    shift(const R& pR, Tag<N> t)
    {
        return SU2ShifterByOne<R,N>(pR);
    }
//
    struct SU2ShiftTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2shift_test.h"
#endif


#endif
