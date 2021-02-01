#ifndef TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITEITERATIONTESTER_TEST_H
#define TEMPLAT_LATTICE_NPARTITEITERATORS_NPARTITEITERATIONTESTER_TEST_H
 
/* This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md. */ 
   
// File info: Main contributor(s): Adrien Florio,  Year: 2020

#include "TempLat/lattice/memory/memorytoolbox.h"
#include "TempLat/lattice/loopers/npartiteloopers/npartitelooper.h"



inline void TempLat::NPartiteLooperTester::Test(TempLat::TDDAssertion& tdd) {


    auto myLittleLambda = [&tdd](int parity, int nDim, int nPoints, int nGhosts){
        auto toolBox = MemoryToolBox::makeShared(nDim, nPoints ,nGhosts);

        auto testCoord = std::make_shared<NPartiteCoordinates>(parity, toolBox->mLayouts.getConfigSpaceJumps(), toolBox->mLayouts.getConfigSpaceLayout());
        NPartiteLooper it(testCoord);
        bool testBool = true;
        int count = 0;

        //for(it.parityBegin(); it.parityEnd(); it.parityMove()){
        it.parityBegin();
            count = 0;
            for(it.begin(); it.end(); ++it)
            {

                auto test = it.getVec();

                ptrdiff_t sum = 0;
                for(size_t i = 0; i< test.size(); ++i)
                {
                    sum += test[i];
                }




                testBool = testBool && (((sum%parity)+parity)%parity == it.getCurrentParity());
                count++;
            }
            tdd.verify( testBool );
            auto reducedSum = toolBox->mGroup.getBaseComm().computeAllSum(count); //MPI friendly test.
            tdd.verify( reducedSum == pow(nPoints,nDim)/parity );
     //   }
    };

    myLittleLambda(2,2,120,0);
    myLittleLambda(2,3,24,4);
    myLittleLambda(2,4,24,2);
    myLittleLambda(2,5,12,1);

    myLittleLambda(4,2,120,0);
    myLittleLambda(4,3,24,4);
    myLittleLambda(4,4,24,2);
    myLittleLambda(4,5,12,1);


   myLittleLambda(3,3,120,5);
    myLittleLambda(3,3,24,4);
    myLittleLambda(3,4,12,2);
    myLittleLambda(3,5,12,1);

}

#endif
