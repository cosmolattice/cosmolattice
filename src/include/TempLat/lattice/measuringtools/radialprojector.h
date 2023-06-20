#ifndef TEMPLAT_LATTICE_MEASUREMENTS_RADIALPROJECTOR_H
#define TEMPLAT_LATTICE_MEASUREMENTS_RADIALPROJECTOR_H

/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */

// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <algorithm>

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionresult.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialprojectionsinglequantity.h"
#include "TempLat/lattice/measuringtools/projectionhelpers/radialbincomputer.h"
#include "TempLat/lattice/algebra/operators/squareroot.h"


#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/getvalue.h"
#include "TempLat/lattice/algebra/helpers/getfloattype.h"
#include "TempLat/lattice/algebra/spacestateinterface.h"
#include "TempLat/lattice/algebra/helpers/confirmspace.h"
#include "TempLat/lattice/algebra/helpers/ghostshunter.h"


namespace TempLat {

    /** \brief A class which projects any N-D lattice on its positive-definite radial coordinate. In other
     *  words, integrating out all the angular dimensions.
     *  When in Fourier-space, this routine takes into account what the redundancies are.
     *  Only the unique values are counted. Real-valued entries get weight 0.5
     *  (they only contribute one float value out of the two for each complex value),
     *  and of the hermitian pairs, only the positivePartner is taken into account.
     *
     *  See `RadialProjectionResult` for a description of the result type.
     *
     * Unit test: make test-radialprojector
     **/

    template <typename T>
    class RadialProjector {
    public:

        typedef typename GetGetReturnType<T>::type vType;
        typedef typename GetFloatType<vType>::type sType;

        static constexpr bool isComplexValued = GetGetReturnType<T>::isComplex;
        typedef typename RadialProjectionResult<sType>::floatType floatType;
        typedef RadialProjectionResult<sType> resultType;

        RadialProjector(const T& instance, SpaceStateInterface::SpaceType spaceType, std::shared_ptr<MemoryToolBox> pToolBox, bool pUseCentralBinValues) :
        mSpaceType(spaceType),
        mInstance(instance),
        mToolBox(pToolBox),
        mUseBinCentralValues(pUseCentralBinValues)
        {

        }


        /** \brief Measure the projection of your n-dimensional lattice onto the
         *  radial direction. Only supports linear binning, so if you want to
         *  transform to e.g. logarithmic binning, you will simply have a pass
         *  a large number of linear bins here, and do the logarithmic binning yourself.
         *  Default is nGrid ^ (nDimensions - 1).
         */
        RadialProjectionResult<sType> measure(ptrdiff_t nLinearBins = -1, sType customRange = -1, bool excludeOrigin = true) {
            if ( nLinearBins < 0 ) {
                ptrdiff_t nGrid = getLayout().getGlobalSizes()[0];
                ptrdiff_t nDim =  getLayout().getGlobalSizes().size();
                nLinearBins = std::pow(nGrid, std::max((ptrdiff_t)1, nDim - 1));
            }




            RadialProjectionResult<sType> baseWorkSpace(nLinearBins, mUseBinCentralValues,  mSpaceType == SpaceStateInterface::SpaceType::Fourier);

            sType minValue = excludeOrigin ? 1.0 : 0.0;

            RadialProjectionResult<sType> myResult = mSpaceType == SpaceStateInterface::SpaceType::Configuration ?
            computeConfigurationSpace(makeBinComputer(nLinearBins, minValue, customRange), baseWorkSpace, excludeOrigin) :
            computeFourierSpace(makeBinComputer(nLinearBins, minValue, customRange), baseWorkSpace, excludeOrigin);

            myResult.finalize(mToolBox->mGroup.getBaseComm());

            return myResult;
        }


    private:
        SpaceStateInterface::SpaceType mSpaceType;
        T mInstance;
        std::shared_ptr<MemoryToolBox> mToolBox;
        bool mUseBinCentralValues;


        const auto& getLayout()
        {
          return mSpaceType == SpaceStateInterface::SpaceType::Fourier ? mToolBox->mLayouts.getFourierSpaceLayout() : mToolBox->mLayouts.getConfigSpaceLayout();
        }


        template <typename BINCOMPUTETYPE>
        auto computeConfigurationSpace(BINCOMPUTETYPE binComputer, RadialProjectionResult<sType> baseWorkSpace, bool excludeOrigin ) {

            auto it = mToolBox->itX();
            confirmGetterSpace();
            for(it.begin();it.end();++it)
            {
                if((not excludeOrigin) or (not it.isAtOrigin())) {

                    sType r = rFromCoords(it.getVec());

                    ptrdiff_t bin = binComputer(r);



                    baseWorkSpace.add(bin, GetValue::get(mInstance, it()), r, 1);
                }
            }
            binComputer.setCentralBinBounds(baseWorkSpace.getCentralBinBounds());

            return baseWorkSpace;
        }

        template <typename BINCOMPUTETYPE>
        auto computeFourierSpace(BINCOMPUTETYPE binComputer, RadialProjectionResult<sType> baseWorkSpace, bool excludeOrigin ) {

            auto it = mToolBox->itP();
            auto layout = getLayout();
            HermitianRedundancy quality;

            confirmGetterSpace();
            for(it.begin();it.end();++it)
            {
              if((not excludeOrigin) or (not it.isAtOrigin())){
                  quality=layout.getHermitianPartners()->qualify(it.getVec());
                 
                  if ( quality != HermitianRedundancy::negativePartner ) {

                      sType r = rFromCoords(it.getVec());

                      ptrdiff_t bin = binComputer(r);

                      /* don't over-weight the real-valued entries: only one float value, only half the weight. */
                      floatType weight = quality == HermitianRedundancy::realValued ? 0.5 : 1;
                      baseWorkSpace.add(bin, GetValue::get(mInstance, it()), r, weight);
                  }
              }
            }
            binComputer.setCentralBinBounds(baseWorkSpace.getCentralBinBounds());

            return baseWorkSpace;
        }

        void confirmGetterSpace()
        {
            ConfirmSpace::apply(mInstance,getLayout(),mSpaceType);
            GhostsHunter::apply(mInstance);
        }

        /** \brief Creates the lambda that maps the IterationCoordinates to a bin. */
        inline auto makeBinComputer(ptrdiff_t nLinearBins, sType minValue, sType customRange = -1) {
            auto layout = getLayout();

            auto rMax = customRange < 0 ? layout.getMaxRadius() : customRange;

            return RadialBinComputer(minValue, rMax, nLinearBins);
        }

        floatType rFromCoords(const std::vector<ptrdiff_t> & pCoord) {
            using namespace std;
            floatType r2 = 0;
            for ( auto&& it : pCoord ){
              r2 += it * it;
            //  say << it;
            }
            return sqrt(r2);
        }


    };

    template <typename T>
    RadialProjector<T> projectRadially(T instance, SpaceStateInterface::SpaceType spaceType, std::shared_ptr<MemoryToolBox> pToolBox, bool useBinCentralValues) {
        return RadialProjector<T>(instance, spaceType, pToolBox, useBinCentralValues);
    }

    template <typename T>
    RadialProjector<T> projectRadially(T instance, bool useBinCentralValues = false) {
        return projectRadially(instance,SpaceStateInterface::SpaceType::Configuration,GetToolBox::get(instance), useBinCentralValues);
    }

    template <typename T>
    RadialProjector<T> projectRadiallyFourier(T instance, bool useBinCentralValues = false) {
        return projectRadially(instance,SpaceStateInterface::SpaceType::Fourier,GetToolBox::get(instance), useBinCentralValues);
    }


    struct RadialProjectorTester {
#ifdef TEMPLATTEST
        static inline void Test(TDDAssertion& tdd);
#endif
    };
}

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/radialprojector_test.h"
#endif


#endif
