#ifndef TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_RANDOMFIELD_TEST_H
#define TEMPLAT_LATTICE_FIELD_INTEGRATIONTESTING_RANDOMFIELD_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Wessel Valkenburg,  Year: 2019

#include <chrono>

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/field/field.h"

inline void TempLat::IntegrationTesting::RandomField::Test(TempLat::TDDAssertion& tdd) {


    say << "Testing C2R -> R2C with a random gaussian. Actually more a test of HermitianPartners and FFTNormalization than anything else.\n";

    auto&& myLittleLambda = [&tdd](ptrdiff_t nDim, ptrdiff_t nGrid, ptrdiff_t nGhost, bool forbidTransposition) {

        auto toolBox = MemoryToolBox::makeShared(nDim, nGrid, nGhost, forbidTransposition);

        auto HP = toolBox->mLayouts.getFourierSpaceLayout().getHermitianPartners();
        say << toolBox->mLayouts << "\n";

//        toolBox->setVerbose();

        Field<double> phi("phi", toolBox);
        Field<double> control_phi("control_phi", toolBox);

        auto rnd = 1*RandomGaussianField<double>("hoi",toolBox);

        phi.inFourierSpace() = rnd;
        control_phi.inFourierSpace() = rnd;

        auto phiMan = phi.getMemoryManager();
        auto control_phiMan = control_phi.getMemoryManager();

        //std::atomic<bool> allEqual(true);
        bool allEqual = true;


        auto it = toolBox->itP();

        for(it.begin(); it.end(); ++it)
        {
          auto a = phiMan->complex(it());
          auto b = control_phiMan->complex(it());
          //        sayMPI << pIterCoords << ", " << HP->qualify(pIterCoords.getCoordinates()) << " a: " << a << ", b: " << b << "\n";
          //bool thisResult = AlmostEqual(a, b, 1e-4);
          allEqual *= AlmostEqual(a, b, 1e-4);
          //bool expected = true;
          //allEqual.compare_exchange_weak(expected, thisResult);
        }




        /* test that the prng repeats itself as it should. */
        tdd.verify( allEqual );


//        /** FFT to configuration space. */
        phi.confirmSpace( toolBox->mLayouts.getConfigSpaceLayout(), SpaceStateInterface::SpaceType::Configuration);
//
//        /** FFT to fourier space. */
        phi.inFourierSpace().confirmSpace( toolBox->mLayouts.getFourierSpaceLayout(), SpaceStateInterface::SpaceType::Fourier);
//
//        //    std::atomic<bool> allEqual(true);
//

        for(it.begin(); it.end(); ++it)
        {
            auto a = phiMan->complex(it());
            auto b = control_phiMan->complex(it());
            allEqual *= AlmostEqual(a, b, 1e-4);

            if ( ! AlmostEqual(a, b, 1e-4) ) {
              auto coords = toolBox->getCoordConfiguration(it());
              std::string coordsStr("(");
              for(size_t j = 0; j<coords.size() ; ++j) coordsStr+=std::to_string(coords[j])+" ";
              coordsStr+=")";
                sayMPI <<  coordsStr << ", " << HP->qualify(toolBox->getCoordFourier(it())) << " a: " << a << ", b: " << b << "\n";
            }
        //    bool expected = true;
        //    allEqual.compare_exchange_weak(expected, thisResult);
        //    return (bool) allEqual;
        }

        tdd.verify( allEqual );
        return (bool) allEqual;
    };
//
//    /* The example case where PFFT fails if we allow for transposed complex layouts. */
      myLittleLambda(3, 4, 0, 0);
////    return;
//    bool goOn = true;
//    for ( ptrdiff_t iDim = 2; iDim < 5 && goOn; ++iDim) {
//        for (ptrdiff_t iSize = 0; iSize < 2 && goOn; ++iSize) {
//            for ( ptrdiff_t iGhost = 0; iGhost < 2 && goOn; ++iGhost) {
//                for (ptrdiff_t iTrans = 0; iTrans < 2 && goOn; ++iTrans) {
//
//                    goOn = myLittleLambda(iDim, 4 + 20 * iSize, iGhost, iTrans);
//                    goOn = MPICommReference().computeAllAnd(goOn);
//                    if ( ! goOn ) {
//                        sayMPI << "Interrupting RandomField::Test, after failing at  myLittleLambda(" << iDim << ", " << 4 + 20 * iSize << ", " << iGhost << ", " << iTrans << "); \n";
//                    }
//                }
//            }
//        }
//    }

}

#endif
