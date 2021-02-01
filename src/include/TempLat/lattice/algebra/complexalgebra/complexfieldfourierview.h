#ifndef TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDFOURIERVIEW_H
#define TEMPLAT_LATTICE_ALGEBRA_COMPLEXALGEBRA_COMPLEXFIELDFOURIERVIEW_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/views/fieldviewfourier.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/tagliteral.h"

namespace TempLat {


    /** \brief A class which holds complex field in fourier space.
     *
     * 
     * Unit test: make test-complexfieldfourierview
     **/

    template <typename T>
    class ComplexFieldFourierView {
    public:
        /* Put public methods here. These should change very little over time. */
        ComplexFieldFourierView(FourierView<T> r, FourierView<T> i) : mR(r), mI(i) {

        }

        std::string toString() const
        {
            return "(" + mR.toString() + ", " + mI.toString() + ")";
        }


        auto ComplexFieldGet(Tag<0> t) {
            return mR;
        }

        auto ComplexFieldGet(Tag<1> t) {
            return mI;
        }

        auto ComplexFieldGet(Tag<0> t, ptrdiff_t i) {
            return mR.get(i);
        }

        auto ComplexFieldGet(Tag<1> t, ptrdiff_t i) {
            return mI.get(i);
        }

        template<int N>
        auto operator()(Tag<N> t) {
            return ComplexFieldGet(t);
        }

        template<typename R>
        void operator=(R &&r) {
            ForLoop(i, 0, size - 1,
                    this->ComplexFieldGet(i).onBeforeAssignment(std::remove_reference<R>::type::Getter::get(r, i)););

            auto it = mR.getToolBox()->itP();

            std::array<std::complex<T>,size> tmpArr;
            auto tmpR = r.ComplexFieldGet(0_c);
            auto tmpI = r.ComplexFieldGet(1_c);

            for(it.begin(); it.end(); ++it){
                auto i = it();

                //DoEval::eval(r,i);
                //tmpArr[0] = r.ComplexFieldGet(0_c,i);
                //tmpArr[1] = r.ComplexFieldGet(1_c,i);
                tmpArr[0] = tmpR.get(i);
                tmpArr[1] = tmpI.get(i);

                mR.getSet(i) = tmpArr[0];
                mI.getSet(i) = tmpArr[1];

            }
            //mR.setGhostsAreStale();
            //mI.setGhostsAreStale();
        }

        inline auto getDx() const
        {
            return mR.getDx();
        }

        inline auto getKIR() const
        {
            return mR.getKIR();
        }

        using Getter = ComplexFieldGetter;
        static constexpr size_t SHIFTIND = 0;
        static constexpr size_t size = 2;



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        FourierView<T> mR;
        FourierView<T> mI;


    };

    class ComplexFieldFourierViewTester{
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/complexfieldfourierview_test.h"
#endif


#endif
