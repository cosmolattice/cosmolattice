#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELD_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexfieldget.h"
#include "TempLat/lattice/algebra/helpers/getdx.h"
#include "TempLat/lattice/algebra/helpers/getkir.h"
#include "TempLat/lattice/algebra/complexalgebra/complexwrapper.h"
#include "TempLat/lattice/algebra/complexalgebra/complexfieldfourierview.h"

namespace TempLat {


    /** \brief A class which implements complex-valued fields.
     *
     * 
     * Unit test: make test-complexfield
     **/
    template<typename T>
    class ComplexField {
    public:
        /* Put public methods here. These should change very little over time. */
        ComplexField(Field<T> f1, Field<T> f2) :
                mR(f1), mI(f2), mName("NoName") {

        }

        ComplexField(std::string name, std::shared_ptr<MemoryToolBox> toolBox, LatticeParameters<T> pLatPar = LatticeParameters<T>()) :
                mR("Re_" + name, toolBox, pLatPar), mI("Im_" + name, toolBox, pLatPar), mName(name) {

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

        auto ComplexFieldGet(ptrdiff_t i) {
            return std::array<T,2>{mR.get(i), mI.get(i)};
        }

        auto& ComplexFieldGetSet(Tag<0> t, ptrdiff_t i) {
            return mR.getSet(i);
        }

        auto& ComplexFieldGetSet(Tag<1> t, ptrdiff_t i) {
            return mI.getSet(i);
        }

        template<int N>
        auto operator()(Tag<N> t) {
            return ComplexFieldGet(t);
        }

        ComplexFieldFourierView<T> inFourierSpace()
        {
            return {mR.inFourierSpace(), mI.inFourierSpace()};
        }


        template<typename R>
        void operator=(R &&r) {
            ForLoop(i, 0, size - 1,
                    this->ComplexFieldGet(i).onBeforeAssignment(std::remove_reference<R>::type::Getter::get(r, i)););

            auto it = mR.getToolBox()->itX();

            std::array<T,size> tmpArr;
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
            mR.setGhostsAreStale();
            mI.setGhostsAreStale();
        }


        template<typename R>
        void operator+=(R &&r) {
            (*this) = (*this) + r;
        }


        std::string toString() const
        {
            return "(" + mR.toString() + ", " + mI.toString() + ")";
        }


        inline auto getDx() const
        {
            return mR.getDx();
        }

        inline auto getKIR() const
        {
            return mR.getKIR();
        }

        void updateGhosts()
        {
            mR.updateGhosts();
            mI.updateGhosts();
        }

        void setGhostsAreStale()
        {
            mR.setGhostsAreStale();
            mI.setGhostsAreStale();
        }



        using Getter = ComplexFieldGetter;
        static constexpr size_t SHIFTIND = 0;
        static constexpr size_t size = 2;


    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        Field<T> mR;
        Field<T> mI;

        const std::string mName;


    };


    template<typename T>
    auto CField(Field<T> f1, Field<T> f2) {
        return ComplexField<T>(f1, f2);
    }


} /* TempLat */



#endif
