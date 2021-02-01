#ifndef COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDAVERAGER_H
#define COSMOINTERFACE_COMPLEXFIELDALGEBRA_COMPLEXFIELDAVERAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/complexalgebra/helpers/hascomplexfieldget.h"
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
#include "TempLat/lattice/algebra/complexalgebra/helpers/complexgetgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"

namespace TempLat {


    /** \brief A class which averages complex fields.
     *
     * 
     * Unit test: make test-complexfieldaverager
     **/

    template <typename T>
    class ComplexFieldAverager {
    public:

        typedef typename ComplexGetGetReturnType<T>::type vType;
        static constexpr bool isComplexValued = IsComplexType<vType>::value;
        static constexpr size_t size = tuple_size<T>::value;

        typedef std::array<vType,size> arrVType;

        /* Put public methods here. These should change very little over time. */
        ComplexFieldAverager(const T& pT,SpaceStateInterface::SpaceType spaceType):
                mT(pT),
                mSpaceType(spaceType)
        {
        }

        arrVType compute()
        {
            arrVType selfResult = mSpaceType == SpaceStateInterface::SpaceType::Fourier ?
                                  computeFourierSpace() :
                                  computeConfigurationSpace() ;
            auto toolBox = mT.ComplexFieldGet(0_c).getToolBox();

            arrVType reducedRes, ret;//= mT.getToolBox()->mGroup.getBaseComm().computeAllSum(selfResult);

            for_in_range<0,size>([&](auto i){
                reducedRes[i] = toolBox->mGroup.getBaseComm().computeAllSum(selfResult[i]);
                ret[i] =AveragerHelper<vType ,isComplexValued>::normalize(toolBox,mSpaceType,reducedRes[i]);
            });

            return ret;
        }

        arrVType computeConfigurationSpace() {
            auto it = mT.ComplexFieldGet(0_c).getToolBox()->itX();
            arrVType mWorkspace{};
            //for_each(mT,[&](auto x){AveragerHelper<vType ,isComplexValued>::onBeforeAverageConfiguration(x,mSpaceType);});
            ForLoop(i, 0, size-1, (AveragerHelper<vType ,isComplexValued>::onBeforeAverageConfiguration(mT.ComplexFieldGet(i),mSpaceType)));
            ptrdiff_t i=0;

            for(it.begin();it.end();++it)
            {
                i = it();

                DoEval::eval(mT,i);

                ForLoop(j, 0, 1, mWorkspace[j] += mT.ComplexFieldGet(j,i));

            }
            return mWorkspace;
        }

        arrVType computeFourierSpace() {
            auto toolBox = mT.ComplexFieldGet(0_c).getToolBox();
            auto it = mT.ComplexFieldGet(0_c).getToolBox()->itP();
            arrVType mWorkspace{};
            ForLoop(i, 0, size-1, (AveragerHelper<vType ,isComplexValued>::onBeforeAverageFourier(mT.ComplexFieldGet(i),mSpaceType)));


            ptrdiff_t i = 0;


            for(it.begin();it.end();++it)
            {
                if ( toolBox->mLayouts.getFourierSpaceLayout().getHermitianPartners()->qualify(it.getVec()) != HermitianRedundancy::negativePartner ){
                    i = it();

                    DoEval::eval(mT,i);
                    ForLoop(j, 0, 1, mWorkspace[j] += mT.ComplexFieldGet(j,i));
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

    class ComplexFieldAveragerTester {
    public:
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };

    template <typename T>
    typename std::enable_if<HasComplexFieldGet<T>::value , decltype(make_list_from_array(std::declval<std::array<typename ComplexGetGetReturnType<T>::type,T::size>>())) >::type
    //auto
    complexfieldaverage(T instance, SpaceStateInterface::SpaceType spaceType = IsComplexType<typename ComplexGetGetReturnType<T>::type>::value ?  SpaceStateInterface::SpaceType::Fourier : SpaceStateInterface::SpaceType::Configuration) {
        return make_list_from_array(ComplexFieldAverager<T>(instance, spaceType).compute());
    }

} /* TempLat */

#ifdef TEMPLATTEST
#include "TempLat/lattice/algebra/complexalgebra/complexfieldaverager_test.h"
#endif


#endif
