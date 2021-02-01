#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLET_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLET_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/field/assignablefieldcollection.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletget.h"
#include "TempLat/util/rangeiteration/make_list_tag.h"


namespace TempLat {


    /** \brief A class which implements su2doublets.
     *
     * 
     * Unit test: make test-su2doublet
     **/
    template<typename T>
    class SU2DoubletBase {//: public CollectionBase<SU2DoubletBase<T,ISMOMENTUM,I1,I2,I3,I4>,T, ISMOMENTUM, I1, I2, I3, I4> {
    public:
        /* Put public methods here. These should change very little over time. */
        SU2DoubletBase(Field<T> f1, Field<T> f2, Field<T> f3 , Field<T> f4) :
                fs{f1,f2,f3,f4},mName("NoName")
        //CollectionBase<SU2DoubletBase<T,ISMOMENTUM,I1,I2,I3,I4>,T, ISMOMENTUM, I1, I2, I3, I4>(f1, f2, f3, f4)
        {

        }
        SU2DoubletBase(std::string name, std::shared_ptr<MemoryToolBox> toolBox, LatticeParameters<T> pLatPar) : mName(name) //:
               // CollectionBase<SU2DoubletBase<T,ISMOMENTUM,I1,I2,I3,I4>,T,ISMOMENTUM,I1,I2,I3,I4>(name+"^", toolBox, pLatPar)
        {
            for(size_t i = 0; i<4 ;++i) fs.emplace_back(Field<T>(name + "_" + std::to_string(i),toolBox,pLatPar));
        }


        template <int N>
        auto SU2DoubletGet(Tag<N> t)
        {
            return (*this)(t);
        }

        template <int N>
        auto SU2DoubletGet(Tag<N> t, ptrdiff_t i)
        {
            return fs[t].get(i);
        }

        template<int M>
        auto operator()(Tag<M> t)
        {
            return fs[t];
        }



        template <typename R>
        void operator=(R&& r)
        {
          /*  for_in_range<0,std::remove_reference<R>::type::size>(
                    [&](auto i)
                    {
                        (*this)(i) = std::remove_reference<R>::type::Getter::get(r,i);
                    }
            );*/

            {
                ForLoop(i, 0, size-1, this->SU2DoubletGet(i).onBeforeAssignment(std::remove_reference<R>::type::Getter::get(r,i)););

                auto it = SU2DoubletGet(0_c).getToolBox()->itX();
                std::array<T,size> tmpArr;

                //auto getter = MakeArray(j,0,size-1,std::remove_reference<R>::type::Getter::get(r,j));
                for(it.begin(); it.end(); ++it){
                    auto i = it();
                    DoEval::eval(r,i);
                   // ForLoop(j,0, size-1,tmpArr[j] =  r.SU2DoubletGet(j,i));
                   // ForLoop(j,0, size-1,SU2DoubletGet(j).getSet(i) = tmpArr[j]);
                    tmpArr[0] = r.SU2DoubletGet(0_c,i);
                    tmpArr[1] = r.SU2DoubletGet(1_c,i);
                    tmpArr[2] = r.SU2DoubletGet(2_c,i);
                    tmpArr[3] = r.SU2DoubletGet(3_c,i);

                    fs[0].getSet(i) = tmpArr[0];
                    fs[1].getSet(i) = tmpArr[1];
                    fs[2].getSet(i) = tmpArr[2];
                    fs[3].getSet(i) = tmpArr[3];
                }
                ForLoop(j,0,size-1, this->SU2DoubletGet(j).setGhostsAreStale());
            }
        }

        template <typename R>
        void operator+=(R&& r){
            (*this) = (*this) + r;
        }


        std::string toString() const {
            return mName;
        }

        std::shared_ptr<MemoryToolBox> getToolBox()
        {
            return GetToolBox::get(fs[0]);
        }


        inline auto getDx() const
        {
            return GetDx::getDx(fs[0]);
        }

        inline auto getKIR() const
        {
            return GetKIR::getKIR(fs[0]);
        }

        using Getter = SU2DoubletGetter;
        static constexpr size_t SHIFTIND = 0;
        static constexpr size_t size = 4;



    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */

        std::vector<Field<T>> fs;

        const std::string mName;


    public:
    };

    template<typename T>
    auto SU2Doubl(Field<T> f1, Field<T> f2, Field<T> f3 , Field<T> f4)
    {
        return 0;//SU2DoubletBase<T>(f1, f2, f3, f4);

    }

    template<typename T>
    using SU2Doublet = SU2DoubletBase<T>;

    struct SU2DoubletTester{
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif

    };

} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doublet_test.h"
#endif


#endif
