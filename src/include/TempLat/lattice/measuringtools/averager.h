#ifndef TEMPLAT_LATTICE_MEASUREMENTS_AVERAGER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_AVERAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"

namespace TempLat {

    /** \brief A class which computes the average value of a getter.
     *
     *
     * Unit test: make test-averager
     **/


  template<typename T>
  class Averager{
  public:
      typedef typename GetGetReturnType<T>::type vType;
      static constexpr bool isComplexValued = GetGetReturnType<T>::isComplex;

      /* Put public methods here. These should change very little over time. */
      Averager(const T& pT,SpaceStateInterface::SpaceType spaceType):
      mT(pT),
      mSpaceType(spaceType)
      {
      }

      operator vType()
      {
        return compute();
      }

      vType compute()
      {
        vType selfResult = mSpaceType == SpaceStateInterface::SpaceType::Fourier ?
            computeFourierSpace() :
            computeConfigurationSpace() ;

        vType reducedRes = mT.getToolBox()->mGroup.getBaseComm().computeAllSum(selfResult);

        return AveragerHelper<vType ,isComplexValued>::normalize(mT.getToolBox(),mSpaceType,reducedRes);
      }

      vType computeConfigurationSpace() {
        ptrdiff_t i = 0;
        auto& it = mT.getToolBox()->itX();
        vType mWorkspace = 0;
        AveragerHelper<vType,isComplexValued>::onBeforeAverageConfiguration(mT,mSpaceType);
        for(it.begin();it.end();++it)
        {
            i = it();
            DoEval::eval(mT,i);
            mWorkspace+=GetEval::getEval(mT,i);
        }
          return mWorkspace;
      }

      vType computeFourierSpace() {
        auto& it=mT.getToolBox()->itP();
        vType mWorkspace=0;
          AveragerHelper<vType,isComplexValued>::onBeforeAverageFourier(mT,mSpaceType);
        for(it.begin();it.end();++it)
        {
            DoEval::eval(mT,it());
            if ( mT.getToolBox()->mLayouts.getFourierSpaceLayout().getHermitianPartners()->qualify(it.getVec()) != HermitianRedundancy::negativePartner )
            mWorkspace+=GetEval::getEval(mT,it());
        }
          return mWorkspace;
      }

      std::string toString() const {
          return "<" + GetString::get(mT) + ">";
      }

      /** For measurement objects: need the toolbox for easiest access to loopers and whatever else. */
      virtual inline
      std::shared_ptr<MemoryToolBox> getToolBox() {
          return GetToolBox::get(mT);
      }

  private:
      /* Put all member variables and private methods here. These may change arbitrarily. */
      T mT;
      SpaceStateInterface::SpaceType mSpaceType;
  };

  /*template <typename T>
  auto average(T instance, SpaceStateInterface::SpaceType spaceType = GetGetReturnType<T>::isComplex ?  SpaceStateInterface::SpaceType::Fourier : SpaceStateInterface::SpaceType::Configuration) {
      return Averager<T>(instance, spaceType).compute();
  }*/

    template <typename T>
    typename std::enable_if<!IsTempLatGettable<0,T>::value, typename GetGetReturnType<T>::type>::type
    average(T instance, SpaceStateInterface::SpaceType spaceType = GetGetReturnType<T>::isComplex ?  SpaceStateInterface::SpaceType::Fourier : SpaceStateInterface::SpaceType::Configuration) {
        return Averager<T>(instance, spaceType).compute();
    }

    auto average(ZeroType a)
    {
        return 0;
    }

    template <typename T>
    auto getAverager(T instance, SpaceStateInterface::SpaceType spaceType = GetGetReturnType<T>::isComplex ?  SpaceStateInterface::SpaceType::Fourier : SpaceStateInterface::SpaceType::Configuration) {
        return Averager<T>(instance, spaceType);
    }


  struct AveragerTester {
  public:
#ifdef TEMPLATTEST
      static inline void Test(TDDAssertion& tdd);
#endif
  };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/averager_test.h"
#endif


#endif
