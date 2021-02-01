#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDSHIFT_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/operators/shift.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldunaryoperator.h"
#include "TempLat/util/rangeiteration/tagliteral.h"


namespace TempLat {


    /** \brief A class which implements spatial shifts for complex algebra.
     *
     *
     * Unit test: make test-su2shift
     **/

    template <typename R, int... N>
    class ComplexFieldShifter : public ComplexFieldUnaryOperator<R>{
    public:
        using ComplexFieldUnaryOperator<R>::mR;

        /* Put public methods here. These should change very little over time. */
        ComplexFieldShifter(const R& pR) : ComplexFieldUnaryOperator<R>(pR), shiftInd(shift<N...>(mR.ComplexFieldGet(0_c)).getShift()) {
            shiftString = shift<N...>(mR.ComplexFieldGet(0_c)).getString({N...}) ;

        }

        template<int M>
        auto ComplexFieldGet(Tag<M> t)
        {
            return shift<N...>(mR.ComplexFieldGet(t));
        }

        template<int M>
        auto ComplexFieldGet(Tag<M> t, ptrdiff_t i)
        {
            return mR.ComplexFieldGet(t,i+shiftInd);
        }
        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i+shiftInd);
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
    class ComplexFieldShifterByOne : public ComplexFieldUnaryOperator<R> {
    public:
        /* Put public methods here. These should change very little over time. */

        using ComplexFieldUnaryOperator<R>::mR;

        ComplexFieldShifterByOne(const R& pR) : ComplexFieldUnaryOperator<R>(pR),shiftInd(shift<N>(mR.ComplexFieldGet(0_c)).getShift()) {

        }

        template<int M>
        auto ComplexFieldGet(Tag<M> t)
        {
            return shift<N>(mR.ComplexFieldGet(t));
        }

        template<int M>
        auto ComplexFieldGet(Tag<M> t, ptrdiff_t i)
        {
            return mR.ComplexFieldGet(t, i+shiftInd);
        }

        void eval(ptrdiff_t i)
        {
            DoEval::eval(mR, i+shiftInd);
        }

        std::string toString() const {
            return GetString::get(mR) + "(->" + std::to_string(N)+")";
        }


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        ptrdiff_t shiftInd;
    };


    template <int... shifts, class R>
    typename std::enable_if<(sizeof...(shifts) > 1) && HasComplexFieldGet<R>::value , ComplexFieldShifter<R,shifts...>>::type
    shift(const R& pR)
    {
        return ComplexFieldShifter<R,shifts...>(pR);
    }

    template <int N, class R>
    typename std::enable_if<HasComplexFieldGet<R>::value, ComplexFieldShifterByOne<R,N>>::type
    shift(const R& pR)
    {
        return ComplexFieldShifterByOne<R,N>(pR);
    }

    template < class R, int N>
    typename std::enable_if<HasComplexFieldGet<R>::value, ComplexFieldShifterByOne<R,N>>::type
    shift(const R& pR, Tag<N> t)
    {
        return ComplexFieldShifterByOne<R,N>(pR);
    }

    struct ComplexFieldShiftTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/complexfieldshift_test.h"
#endif


#endif
