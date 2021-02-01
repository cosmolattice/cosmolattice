#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2FIELD_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2get.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"
#include "TempLat/util/rangeiteration/sum_in_range.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"



namespace TempLat {


    /** \brief A class which implements a SU2 field (group).
     *
     * 
     * Unit test: make test-su2field
     **/

    template<typename T>
    class SU2FieldBase
    {
    public:

        /* Put public methods here. These should change very little over time. */
        SU2FieldBase(Field<T> f1, Field<T> f2, Field<T> f3):
        fs{f1,f2,f3},
        cache(0),
        mName("NoName"){

        }

        SU2FieldBase(std::string name, std::shared_ptr<MemoryToolBox> toolBox, LatticeParameters<T> pLatPar):
        cache(0),
        mName(name)
        {
            for(size_t i = 0; i<3 ;++i){
                fs.push_back(Field<T>(name + "_" + std::to_string(i+1),toolBox,pLatPar));
            }
        }

        template <int N>
        auto SU2Get(Tag<N> t)
        {
            return (*this)(t);
        }


        auto operator()(Tag<0> t)
        {
            return sqrt(1.0 - pow<2>(fs[0]) -  pow<2>(fs[1]) -  pow<2>(fs[2]));
        }

        template<int M>
        auto& operator()(Tag<M> t)
        {
            return fs[M-1];
        }


        auto SU2Get(Tag<0> t, ptrdiff_t i)
        {
            return sqrt(1.0 - pow<2>(fs[0].get(i)) - pow<2>(fs[1].get(i)) - pow<2>(fs[2].get(i))); // Apriori not optimal, as we compute several time c0, but does not seem to make a difference.
        }
        template<int M>
        auto SU2Get(Tag<M> t, ptrdiff_t i)
        {
            return fs[M-1].get(i);
        }

       /* auto SU2Get(Tag<0> t, ptrdiff_t i)
        {
            return cache;
        }

        void eval(ptrdiff_t i)
        {
            cache = sqrt(1.0 - pow<2>(fs[0].get(i)) - pow<2>(fs[1].get(i)) - pow<2>(fs[2].get(i)));
        }*/


        template <typename R>
        void operator=(R&& r)
        {
            ForLoop(i, 1, size-1, fs[i-1].onBeforeAssignment(std::remove_reference<R>::type::Getter::get(r,i)););

            auto it = fs[0].getToolBox()->itX();
            std::array<T,size> tmpArr;

            //T det = 0;

            for(it.begin(); it.end(); ++it){
                auto i = it();

                DoEval::eval(r,i);
                tmpArr = r.SU2Get(i);


                //fs[0].getSet(i) = sqrt(1.0 - pow<2>(tmpArr[1]) - pow<2>(tmpArr[2]) - pow<2>(tmpArr[3]));
                //fs[0].getSet(i) = tmpArr[0];

                fs[0].getSet(i) = tmpArr[1];
                fs[1].getSet(i) = tmpArr[2];
                fs[2].getSet(i) = tmpArr[3];

            }
            ForLoop(j,0,2, fs[j].setGhostsAreStale());
        }

        std::string toString() const{
            return mName;
        }


        inline auto getDx() const
        {
            return GetDx::getDx(fs[0]);
        }

        inline auto getKIR() const
        {
            return GetKIR::getKIR(fs[0]);
        }

        using Getter = SU2Getter;
        static constexpr size_t SHIFTIND = 0;
        static constexpr size_t size = 4;
        static constexpr size_t numberToSkipAsTuple = 1;

    protected:

        std::array<T,4> SU2Get(ptrdiff_t i) //This function is private, as it is designed only for internal use and more importantly, it does not return the correct 0th component,
        {                                   // for optmisation purposes.
            return {/*sqrt(1.0 - pow<2>(fs[1].get(i)) - pow<2>(fs[2].get(i)) - pow<2>(fs[3].get(i)))*/0, fs[0].get(i), fs[1].get(i) ,fs[2].get(i)};
        }

        std::vector<Field<T>> fs;
        T cache;
        const std::string mName;

        /* Put all member variables and private methods here. These may change arbitrarily. */

    };


    struct SU2FieldTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template<typename T>
    using SU2Field = SU2FieldBase<T>;

} /* TempLat */

#ifdef TEMPLATTEST
#include "su2field_test.h"
#endif


#endif
