#ifndef COSMOINTERFACE_SU2ALGEBRA_SU2AVERAGER_H
#define COSMOINTERFACE_SU2ALGEBRA_SU2AVERAGER_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Adrien Florio,  Year: 2019

#include "TempLat/lattice/algebra/su2algebra/helpers/hassu2get.h"
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
#include "TempLat/lattice/algebra/su2algebra/helpers/su2getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/operators/power.h"

namespace TempLat
{
  /** \brief An averager for su2. Allows to take into account cached operations consistently.
   *
   * Unit test: make test-su2averager
   **/
  template <typename T> class SU2Averager
  {
  public:
    /* Put public methods here. These should change very little over time. */
    using vType = typename SU2GetGetReturnType<T>::type;
    static constexpr bool isComplexValued = IsComplexType<vType>;
    static constexpr size_t size = tuple_size<T>::value;
    using arrVType = std::array<vType, size>;

    SU2Averager(const T &pT, SpaceStateType spaceType) : mT(pT), mSpaceType(spaceType) {}

    arrVType compute()
    {
      arrVType selfResult = mSpaceType == SpaceStateType::Fourier ? computeFourierSpace() : computeConfigurationSpace();
      auto toolBox = mT.SU2Get(0_c).getToolBox();

      arrVType reducedRes, ret; //= mT.getToolBox()->mGroup.getBaseComm().computeAllSum(selfResult);

      for_in_range<0, size>([&](auto i) {
        reducedRes[i] = toolBox->mGroup.getBaseComm().computeAllSum(selfResult[i]);
        ret[i] = AveragerHelper<vType, isComplexValued>::normalize(toolBox, mSpaceType, reducedRes[i]);
      });

      return ret;
    }

    arrVType computeConfigurationSpace()
    {
      auto it = mT.SU2Get(0_c).getToolBox()->itX();
      arrVType mWorkspace{};
      // for_each(mT,[&](auto x){AveragerHelper<vType ,isComplexValued>::onBeforeAverageConfiguration(x,mSpaceType);});
      ForLoop(i, 0, size - 1,
              (AveragerHelper<vType, isComplexValued>::onBeforeAverageConfiguration(mT.SU2Get(i), mSpaceType)));
      ptrdiff_t i = 0;

      arrVType tmpArr;

      for (it.begin(); it.end(); ++it) {
        i = it();

        DoEval::eval(mT, i);
        tmpArr = mT.SU2Get(i);

        mWorkspace[0] += sqrt(1.0 - pow<2>(tmpArr[1]) - pow<2>(tmpArr[2]) - pow<2>(tmpArr[3]));
        mWorkspace[1] += tmpArr[1];
        mWorkspace[2] += tmpArr[2];
        mWorkspace[3] += tmpArr[3];
      }
      return mWorkspace;
    }

    arrVType computeFourierSpace()
    {
      auto toolBox = mT.SU2Get(0_c).getToolBox();
      auto it = mT.SU2Get(0_c).getToolBox()->itP();
      arrVType mWorkspace{};
      ForLoop(i, 0, size - 1,
              (AveragerHelper<vType, isComplexValued>::onBeforeAverageFourier(mT.SU2Get(i), mSpaceType)));

      arrVType tmpArr;

      ptrdiff_t i = 0;

      for (it.begin(); it.end(); ++it) {
        if (toolBox->mLayouts.getFourierSpaceLayout().getHermitianPartners()->qualify(it.getVec()) !=
            HermitianRedundancy::negativePartner) {
          i = it();

          DoEval::eval(mT, i);
          tmpArr = mT.SU2Get(i);
          mWorkspace[0] += tmpArr[0];
          mWorkspace[1] += tmpArr[1];
          mWorkspace[2] += tmpArr[2];
          mWorkspace[3] += tmpArr[3];
        }
      }
      return mWorkspace;
    }

    std::string toString() const { return "<" + GetString::get(mT) + ">"; }

  private:
    /* Put all member variables and private methods here. These may change arbitrarily. */
    T mT;
    SpaceStateType mSpaceType;
  };

  struct SU2AveragerTester {
#ifdef TEMPLATTEST
    static inline void Test(TDDAssertion &tdd);
#endif
  };

  template <typename T>
    requires HasSU2Get<T>
  auto su2average(T instance, SpaceStateType spaceType = IsComplexType<typename SU2GetGetReturnType<T>::type>
                                                             ? SpaceStateType::Fourier
                                                             : SpaceStateType::Configuration)
  {
    return make_list_from_array(SU2Averager<T>(instance, spaceType).compute());
  }
} // namespace TempLat

#endif
