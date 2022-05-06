#ifndef TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_H
#define TEMPLAT_LATTICE_MEASUREMENTS_MAXIMUM_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2021

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


     MakeException(MaximumWrongSpace);

  template<typename T>
  class Maximum{
  public:
      typedef typename GetGetReturnType<T>::type vType;

      /* Put public methods here. These should change very little over time. */
      Maximum(const T& pT,SpaceStateInterface::SpaceType spaceType):
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
            throw(MaximumWrongSpace("Maximum works only in real space.")) :
            computeConfigurationSpace() ;

        vType reducedRes = mT.getToolBox()->mGroup.getBaseComm().computeAllMax(selfResult);

        return reducedRes;
      }

      vType computeConfigurationSpace() {
        ptrdiff_t i = 0;
        auto& it = mT.getToolBox()->itX();
        vType mWorkspace1 = 0;
        vType mWorkspace2 = std::numeric_limits<vType>::lowest();
        onBeforeAverageConfiguration(mT);
        for(it.begin();it.end();++it)
        {
            i = it();
            DoEval::eval(mT,i);
            mWorkspace1 = GetEval::getEval(mT,i);
            if(mWorkspace1 > mWorkspace2) mWorkspace2 = mWorkspace1;
        }
          return mWorkspace2;
      }

      template <typename R>
      static void onBeforeAverageConfiguration(R&& pT) {
          /* likewise, make sure we are in configuration space (here the FFT may be fired!). */
          ConfirmSpace::apply(pT,pT.getToolBox()->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration );
          GhostsHunter::apply(pT);

      }
      std::string toString() const {
          return "max(" + GetString::get(mT) + ")";
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



    template <typename T>
    typename std::enable_if<!IsTempLatGettable<0,T>::value, typename GetGetReturnType<T>::type>::type
    max(T instance, SpaceStateInterface::SpaceType spaceType = GetGetReturnType<T>::isComplex ?  SpaceStateInterface::SpaceType::Fourier : SpaceStateInterface::SpaceType::Configuration) {
        return Maximum<T>(instance, spaceType).compute();
    }

    auto max(ZeroType a)
    {
        return 0;
    }



  struct MaximumTester {
  public:
#ifdef TEMPLATTEST
      static inline void Test(TDDAssertion& tdd);
#endif
  };

}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/maximum_test.h"
#endif


#endif
