#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETAVERAGER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2DOUBLETAVERAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2doubletget.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/rangeiteration/for_in_range.h"
#include "TempLat/util/rangeiteration/make_tuple_tag.h"
#include "TempLat/util/rangeiteration/tagliteral.h"
#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/algebra/helpers/getcomponent.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"
#include "TempLat/lattice/algebra/helpers/istemplatgettable.h"
#include "TempLat/lattice/algebra/su2algebra/helpers/su2doubletgetgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"


namespace TempLat {


    /** \brief An averager specialised for SU2Doublet. Allows to take into account cached operations consistently.
     * 
     * 
     * Unit test: make test-su2doubletaverager
     **/

    template<typename T>
    class SU2DoubletAverager {
    public:

        typedef typename SU2DoubletGetGetReturnType<T>::type vType;
        static constexpr bool isComplexValued = IsComplexType<vType>::value;
        static constexpr size_t size = tuple_size<T>::value;

        typedef std::array<vType,size> arrVType;

        /* Put public methods here. These should change very little over time. */
        SU2DoubletAverager(const T& pT,SpaceStateInterface::SpaceType spaceType):
                mT(pT),
                mSpaceType(spaceType)
        {
        }

        arrVType compute()
        {
            arrVType selfResult = mSpaceType == SpaceStateInterface::SpaceType::Fourier ?
                                  computeFourierSpace() :
                                  computeConfigurationSpace() ;
            auto toolBox = mT.SU2DoubletGet(0_c).getToolBox();

            arrVType reducedRes, ret;//= mT.getToolBox()->mGroup.getBaseComm().computeAllSum(selfResult);

            for_in_range<0,size>([&](auto i){
                reducedRes[i] = toolBox->mGroup.getBaseComm().computeAllSum(selfResult[i]);
                ret[i] =AveragerHelper<vType ,isComplexValued>::normalize(toolBox,mSpaceType,reducedRes[i]);
            });

            return ret;
        }

        arrVType computeConfigurationSpace() {
            auto it = mT.SU2DoubletGet(0_c).getToolBox()->itX();
            arrVType mWorkspace{};
            //for_each(mT,[&](auto x){AveragerHelper<vType ,isComplexValued>::onBeforeAverageConfiguration(x,mSpaceType);});
            ForLoop(i, 0, size-1, (AveragerHelper<vType ,isComplexValued>::onBeforeAverageConfiguration(mT.SU2DoubletGet(i),mSpaceType)));
            ptrdiff_t i=0;

            for(it.begin();it.end();++it)
            {
                i = it();

                DoEval::eval(mT,i);

                ForLoop(j, 0, 3, mWorkspace[j] += mT.SU2DoubletGet(j,i));

            }
            return mWorkspace;
        }

        arrVType computeFourierSpace() {
            auto toolBox = mT.SU2DoubletGet(0_c).getToolBox();
            auto it = mT.SU2DoubletGet(0_c).getToolBox()->itP();
            arrVType mWorkspace{};
            ForLoop(i, 0, size-1, (AveragerHelper<vType ,isComplexValued>::onBeforeAverageFourier(mT.SU2DoubletGet(i),mSpaceType)));


            ptrdiff_t i = 0;


            for(it.begin();it.end();++it)
            {
                if ( toolBox->mLayouts.getFourierSpaceLayout().getHermitianPartners()->qualify(it.getVec()) != HermitianRedundancy::negativePartner ){
                    i = it();

                    DoEval::eval(mT,i);
                    ForLoop(j, 0, 3, mWorkspace[j] += mT.SU2DoubletGet(j,i));
                }
            }
            return mWorkspace;
        }

        std::string toString() const {
            return "<" + GetString::get(mT) + ">";
        }
    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */
        T mT;
        SpaceStateInterface::SpaceType mSpaceType;

    private:
        /* Put all member variables and private methods here. These may change arbitrarily. */




    };

    template <typename T>
    typename std::enable_if<HasSU2DoubletGet<T>::value , decltype(make_list_from_array(std::declval<std::array<typename SU2DoubletGetGetReturnType<T>::type,T::size>>())) >::type
    //auto
    su2doubletaverage(T instance, SpaceStateInterface::SpaceType spaceType = IsComplexType<typename SU2DoubletGetGetReturnType<T>::type>::value ?  SpaceStateInterface::SpaceType::Fourier : SpaceStateInterface::SpaceType::Configuration) {
        return make_list_from_array(SU2DoubletAverager<T>(instance, spaceType).compute());
    }


    class SU2DoubletAveragerTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };



} /* TempLat */

#ifdef TEMPLATTEST
#include "su2doubletaverager_test.h"
#endif


#endif
