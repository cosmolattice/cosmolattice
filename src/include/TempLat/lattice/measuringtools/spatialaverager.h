#ifndef TEMPLAT_LATTICE_MEASUREMENTS_SPATIALAVERAGER_H
#define TEMPLAT_LATTICE_MEASUREMENTS_SPATIALAVERAGER_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/util/tdd/tdd.h"

#include "TempLat/util/tdd/tdd.h"
#include "TempLat/util/getcpptypename.h"
#include "TempLat/lattice/algebra/helpers/getgetreturntype.h"
#include "TempLat/lattice/algebra/helpers/geteval.h"
#include "TempLat/lattice/algebra/helpers/doeval.h"
#include "TempLat/lattice/algebra/helpers/getstring.h"
#include "TempLat/lattice/measuringtools/averagerhelper.h"

namespace TempLat {

        /** \brief A class which computes the average value of a getter over the first n-1 coordinates, returns a vector.
         *
         *
         * Unit test: make test-averager
         **/


        template<typename T>
        class SpatialAverager{
        public:
            typedef typename GetGetReturnType<T>::type vType;

            /* Put public methods here. These should change very little over time. */
            SpatialAverager(const T& pT,SpaceStateInterface::SpaceType spaceType):
                    mT(pT),
                    mSpaceType(spaceType)
            {
            }

            operator vType()
            {
                return compute();
            }

            std::vector<vType> compute() // We use the fat that the last dimension is never split in the parallelization
            {

                if(mSpaceType ==SpaceStateInterface::SpaceType::Fourier) throw AveragerWrongSpace("Spatial averager works only in configuration space.");

                std::vector<vType> selfResult = computeConfigurationSpace() ;

                std::vector<vType> reducedRes;
                for(auto x : selfResult) reducedRes.emplace_back(mT.getToolBox()->mGroup.getBaseComm().computeAllSum(x));

                auto tmpDims = mT.getToolBox()-> mNGridPointsVec;
                vType spatialVolume = 1;

                for(size_t i = 0; i < tmpDims.size()-1; ++i) spatialVolume *= tmpDims[i];
                for(auto& x : reducedRes) x /= spatialVolume;

                return reducedRes;
            }

            std::vector<vType> computeConfigurationSpace() {
                ptrdiff_t i = 0;
                auto& it = mT.getToolBox()->itX();
                auto Nt = mT.getToolBox()-> mNGridPointsVec.back();
                ptrdiff_t t = 0;
                std::vector<vType> mWorkspace (Nt,0);
                AveragerHelper<vType,false>::onBeforeAverageConfiguration(mT,mSpaceType);
                for(it.begin();it.end();++it)
                {
                    i = it();
                    t = mT.getToolBox()->getCoordConfiguration0N(i)[mT.getToolBox()->mNDimensions - 1];
                    DoEval::eval(mT,i);
                    mWorkspace[t] += GetEval::getEval(mT,i);
                }
                return mWorkspace;
            }


            std::string toString() const {
                return "<" + GetString::get(mT) + ">_spat";
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
        typename std::enable_if<!IsTempLatGettable<0,T>::value, std::vector<typename GetGetReturnType<T>::type>>::type
        spatialAverage(T instance, SpaceStateInterface::SpaceType spaceType =  SpaceStateInterface::SpaceType::Configuration) {
            return SpatialAverager<T>(instance, spaceType).compute();
        }

        auto spatialAverage(ZeroType a)
        {
            return 0;
        }


        struct SpatialAveragerTester {
        public:
#ifdef TEMPLATTEST
            static inline void Test(TDDAssertion& tdd);
#endif
        };

    }

#ifdef TEMPLATTEST
#include "TempLat/lattice/measuringtools/spatialaverager_test.h"
#endif


#endif
